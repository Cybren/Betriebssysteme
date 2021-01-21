Aufgabe 3: Interruptsynchronisation für OOStuBS/MPStuBS mit dem Pro-/Epilogmodell {#aufgabe3}
=============================================================================================

# Lernziel #

- Schutz kritischer Abschnitte mit Hilfe des Prolog-/Epilogmodells.

# Aufgabenbeschreibung #

Die Synchronisation der Aktivitäten innerhalb von OOStuBS/MPStuBS soll auf
die Verwendung des Pro-/Epilogmodells umgestellt werden. Euer bisheriges
Programm (Aufgabe 2) soll also so verändert werden, dass die
Synchronisation nicht mehr durch Interruptsperren (harte Synchronisation)
erfolgt.

Hierzu müssen die Klassen Keyboard, Panic, Gate und die Funktion guardian()
angepasst bzw. erweitert werden. Die Klassen Guard und Secure kommen
neu dazu. Außerdem ist es natürlich notwendig, dass ihr eure Applikation
entsprechend anpasst.

Außerdem soll ein globales Guard Objekt guard angelegt werden, mit dessen
Hilfe alle kritischen Abschnitte geschützt werden sollen.

In MPStuBS muss der Guard nicht nur gegen Interrupts schützen, sondern auch
andere CPUs aussperren und so am Betreten des kritischen Abschnittes
hindern. Dazu soll der Guard aktiv warten und die Abarbeitung des
kritischen Abschnitts auf diese Weise serialisieren.

\dotfile "a3.dot" "Klassenübersicht für Aufgabe 3"

# Eigenschaften von Prolog und Epilog #

## Gate::prologue() ##

- Der Prolog wird von der Hardware über den Umweg des guardian() aktiviert.
- Der Code im Prolog soll kurz sein und nur die nötigste Behandlung
  der Hardware vornehmen. Es soll sich minimalen Zustand mit dem Rest
  des Systems teilen.
- Die Verwendung eines Prologs reduziert die Interruptlatenz im gesamten System.
- Bei Bedarf fordert ein Prolog einen Epilog an

## Gate::epilogue() ##

- Der Epilog wird nach dem Prolog ausgeführt und ist dessen kausale Folge.
- Die Ausführung von Epilogen wird auf Ebene ½ im gesamten System
  synchronisiert. Das heisst, dass zu jedem Zeitpunkt nur ein
  Kontrollfluss auf Ebene ½ ist.
- Auf Ebene ½ sind Interrupts immer aktiv. Es können Prologe auftreten.
- Die Ausführung von Epilogen ist greedy. Das heisst, dass wenn ein
  Epilog ausgeführt werden kann, er ausgeführt wird. Niemals verlässt
  jemand die Ebene ½, wenn noch Epiloge vorhanden sind.
- Für MPStuBS gilt: Epiloge werden auf der CPU ausgeführt, auf denen
  die Prologe stattgefunden haben.

# Implementierungshinweise #
- Das Testprogramm kann von Aufgabe 2 übernommen werden. Auch diesmal
  sollen wieder Ausgaben an verschiedenen Positionen aus dem
  Hauptprogramm und aus dem Interrupthandler gemacht werden.
- Da die Verwendung des Pro-/Epilogmodells den Schutz kritischer Abschnitte
  mit Hilfe von CPU::disable_int() und CPU::enable_int() und in MPStuBS auch
  mit Hilfe eines eigenen Spinlocks weitgehend überflüssig macht, sollten
  zunächst einmal wieder alle entsprechenden Aufrufe entfernt werden.
- Da bei unserer Implementierung die Interrupts vor Aufruf von guardian()
  gesperrt werden, müssen sie an geeigneter Stelle "von Hand" wieder
  freigegeben werden.
- In MPStuBS können zum Schutz der kritischen Abschnitte im Guard zusätzlich
  zu Interruptsperren die in Aufgabe 2 implementierten Spinlocks verwendet
  werden, um auch gegen andere CPUs synchronisieren zu können.

## Serielle Schnittstelle erweitern (optional) ##
  Die serielle Schnittstelle kann ebenfalls auf den Interruptbetrieb umgestellt
  werden -- dabei begnügen wir uns mit dem Empfangsinterrupt.
  Dadurch kann man nun auch bequem gleichzeitig Eingaben von Keyboard und
  Console empfangen, ohne umständlich nichtblockierende Abfragen zu
  implementieren.
  Außerdem ist es dadurch auch möglich, sich im laufenden Betriebssystem mit
  den GDB_Stub zu verbinden, das System beliebig (durch Tastendruck) zu
  unterbrechen und den Speicher auszulesen.

# Vorgaben #
Zur Erstellung der Epilog-Warteschlange werden die Hilfsklassen Queue und
QueueLink vorgegeben. Bei Verwendung müsst ihr jedoch auf die richtige
Synchronisation achten.
