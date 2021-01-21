Aufgabe 2: Interruptbehandlung für OOStuBS/MPStuBS {#aufgabe2}
==============================================================

# Lernziele #

- Behandlung asynchroner Ereignisse
- Problematik und Schutz kritischer Abschnitte

# Aufgabenbeschreibung #

Es soll eine einfache Interruptbehandlung für die Unterbrechungen durch die
Tastatur vorgesehen werden.

Hierzu müssen die Klassen IOAPIC, Plugbox, Gate, Panic, Keyboard sowie die
Funktion guardian() implementiert werden. Die Klassen CPU und LAPIC sind in
der Vorgabe enthalten.

Um die entsprechenden Geräte überall in OOStuBS/MPStuBS nutzen zu können,
sollen von den Klassen Plugbox, IOAPIC und Keyboard globale Instanzen
der Objekte namens \b plugbox, \b ioapic und \b keyboard angelegt
werden. Ein globales Objekt lapic der Klasse LAPIC exisiert schon, da
dieses während des Startupvorganges benötigt wird (wenn man dieses benutzen
möchte, reicht also eine \b extern Deklaration des Objekts).

\dotfile "a2.dot" "Klassenübersicht für Aufgabe 2"

# Implementierungshinweise #

## Teil A ##

Im ersten Schritt sollte die Klasse IOAPIC implementiert werden. Mit Hilfe
der Klassen IOAPIC, LAPIC und CPU können Unterbrechungen von der Tastatur
zugelassen und behandelt werden (während des Boot-Vorgangs wurden bei der
%CPU und beim I/O-APIC alle Unterbrechungen ausmaskiert). Wenn dies
funktioniert, müsste automatisch bei jedem Drücken und Loslassen einer
Taste die Funktion guardian() aktiviert werden, da die
Interruptvektortabelle im Startup-Code entsprechend initialisiert wurde.
Mit einer Ausgabe in guardian() kann Aufgabenteil A leicht getestet werden -
zumindestens einige Male. Wenn die Zeichen nicht vom Tastaturcontroller
abgeholt werden, läuft der Tastaturpuffer irgendwann voll. Sobald der
Puffer voll ist, sendet der Tastaturcontroller jedoch keine Interrupts
mehr. Deshalb kann es durchaus passieren, dass ihr zunächst nur für ein
oder zwei Tastendrücke Interrupts bekommt. Je nach Umgebung kann es auch
notwendig sein, den Tastaturpuffer vor der Aktivierung der Interrupts einmal
komplett zu leeren.

#### Tipps
- Während der Behandlung einer Unterbrechung braucht ihr euch um
  unerwünschte Interrupts nicht zu sorgen. Der Prozessor schaltet diese
  nämlich automatisch aus, wenn er mit der Behandlung beginnt und lässt sie
  erst wieder zu, wenn die Unterbrechungsbehandlung mit der
  Assembleranweisung iret beendet wird. Das entspricht der letzten
  schließenden Klammer der guardian()-Implementierung.
- Eure Interruptverarbeitung kann natürlich nur funktionieren, wenn OOStuBS/MPStuBS
  auch läuft. Sobald OOStuBS/MPStuBS die main()-Funktion verlässt, kehrt es
  in den Startup-Code zurück, der mittels eines Aufrufs von CPU::die Interrupts
  deaktiviert und die CPU anhält. Ein Betriebssystem sollte halt nicht
  plötzlich enden 😉. Explizit für MPStuBS gilt dies auch für die main_ap()
  Funktion, die von den Applikationsprozessoren ausgeführt wird.
- In einem modernen PC geschieht die Interruptverarbeitung durch ein
  Zusammenspiel des I/O-APICs, an welchen die externen Geräte angeschlossen
  sind, mit den in jeder CPU integrierten Local APICs. Um euch das Leben zu
  vereinfachen, ist die Implementierung der Klasse LAPIC (Local APIC) in der
  Vorgabe enthalten. Darüberhinaus sorgt der Startupcode dafür, dass der
  Local APIC in einem definierten Zustand ist, so dass bei korrekter
  Implementierung der IOAPIC-Klasse die Interruptbehandlung funktionieren
  müsste.
- Laut Spezifikation des Local APICs ist es notwendig, die Bearbeitung eines
  jeden Interrupts zu bestätigen. Dies ist durch Aufruf der Methode
  LAPIC::ackIRQ(), die schon in der Vorgabe enthalten ist, möglich.

## Teil B ##
Im zweiten Schritt wird eine Infrastruktur geschaffen, um die
Unterbrechungsbehandlung an ein zugeordnetes Gerätetreiberobjekt
weiterzuleiten. Zur Verwaltung von Treiberobjekten dient die Klasse
Plugbox, die für jeden möglichen Interrupt einen Zeiger auf ein Gate-Objekt
bereithält. Gate ist eine abstrakte Klasse, die die Schnittstelle aller
Interrupt-behandelnden Treiber beschreibt. Initial werden alle Zeiger der
Plugbox so gesetzt, dass sie auf ein globales Panic-Objekt verweisen.

## Teil C ##
Hier soll die Klasse Keyboard implementiert werden. Sie stellt den
eigentlichen Tastaturtreiber dar. Die Unterbrechungen, die die Tastatur
auslöst, müssen abgefangen und interpretiert werden. Als Ergebnis sollen nach
jedem Tastendruck die entsprechenden Zeichen auf ihrer eigenen Zeile auf
dem Bildschirm dargestellt werden. Die Tastenkombination "Ctrl-Alt-Delete"
soll einen Reboot auslösen.

## Teil D ##
Nun soll das Testprogramm an die Interruptverarbeitung angepasst
werden.

#### OOStuBS
Schreibt ein Testprogramm in Application::action(), das von main() aus
aufgerufen wird. Dieses soll in einer Endlosschleife Ausgaben
(beispielsweise ein int-Zähler) mit Hilfe von CGA_Screen::setpos() und den
Streamoperatoren von O_Stream an einer festen Position im kout-Objekt
erzeugen. Es sollte nun möglich sein, durch das Drücken von Tasten die
Ausgabe "durcheinander" bringen zu können. Überlegt euch, was dabei
passiert, warum es passiert, und vermeidet das Problem mit Hilfe der
Methoden der Klasse CPU.

#### MPStuBS
In MPStuBS sollen im Hauptprogramm aller CPUs Ausgaben an verschiedenen
Stellen des Bildschirms mit Hilfe des globalen kout Objekts durchgeführt
werden (CGA_Screen::setpos() verwenden!). Hier braucht man nicht einmal
Interrupts, um die Ausgabe durcheinander zu bringen (Warum?). Zur Vermeidung
des Problems reichen die Methoden der Klasse CPU alleine nicht aus. Deshalb
soll hier noch zusätzlich die Klasse Spinlock (optional auch Ticketlock)
implementiert werden, um parallele Kontrollflüsse zu synchronisieren. Was
muss bei der Verwendung des Spinlocks beachtet werden? Welche Probleme
können dabei auftreten?

## Teil E (optional) ##
Als freiwillige Zusatzaufgabe kann nun mittels der Unterbrechungbehandlungen
und der seriellen Schnittstelle ein GDB_Stub eingebaut werden, mit dem
auch die Rechner (ohne teure Zusatzhardware) entkäfert werden können.

*Bitte beachten:* Für diese Erweiterung werden sowohl Serial (optional in
Aufgabe 1) als auch Ticketlock benötigt. Änderungen an Serial sind jedoch nicht
notwendig (kein Interruptbetrieb - folgt in der nächsten Aufgabe).

# Vorgaben
Die Implementierung der Klassen CPU und LAPIC haben wir bereits für euch
übernommen. Für die eigentliche Tastaturabfrage könnt ihr eure Klasse
Keyboard_Controller aus Aufgabe 1 wiederverwenden (Hinweis: Die Funktion
Keyboard_Controller::key_hit kann an die nun veränderte Situation angepasst werden).

# Hilfestellung
 - \subpage apic
 - Modul \ref interrupts
 - Modul \ref sync
