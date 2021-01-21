Aufgabe 6: Ereignisbearbeitung und Synchronisation {#aufgabe6}
==============================================================

# Lernziel #
 - Synchronisation mit Hilfe von Semaphoren und anderen Kernobjekten

# Aufgabenbeschreibung #
OOStuBS/MPStuBS soll nun um Synchronisationsobjekte erweitert werden, mit
denen Threads sich gegenseitig über verschiedene Ereignisse informieren
bzw. auf Ereignisse warten können. Folgende Synchronisationsobjekte
sollen in dieser Aufgabe erstellt werden:

- Mit Hilfe von Semaphoren besteht die Möglichkeit, Anwendungsthreads
miteinander zu synchronisieren. Darüberhinaus sollen sie dazu verwendet
werden, Anwendungsthreads bei der Tastaturabfrage zu blockieren, bis eine
Taste gedrückt wurde.
- Durch Bell (Glocke) können Threads sich für eine bestimmte Zeit
schlafen legen.

Hierzu müssen die Klassen Waitingroom, Semaphore, Scheduler,
Bell, Bellringer, Guarded_Bell,
Guarded_Semaphore, Guarded_Keyboard, Keyboard,
Thread, Watch, sowie WakeUp implementiert bzw. angepasst werden.

Abschliessend soll dann noch eine Möglichkeit geschaffen werden, den
Prozessor schlafen zu lassen, wenn keine Benutzerthreads vorhanden sind.

Neue Threads können lediglich durch
Interruptbehandlungsroutinen aktiviert werden. Wenn nach einem Interrupt
wieder neue Threads in der Bereitliste vorhanden sind, soll die Ausführung
fortgeführt werden.

Sowohl in OOStuBS als auch in MPStuBS ist es sinnvoll die Idle-Problematik
mit pro Prozessor allokierten Idle-Threads (Instanzen der Klasse
IdleThread) zu lösen, die eingeplant werden, sobald keine anderen
Threads vorhanden sind. Die Idle-Threads führen dann ein Idle-Loop
aus. Darüberhinaus ist es notwendig schlafende
Prozessoren immer dann zu wecken, wenn ein Thread durch Aufruf von
Scheduler::ready() auf die Bereitliste gesetzt wird. Dazu soll ein eigener
Interprozessorinterrupt (IPI) analog zur Implementierung von
Scheduler::kill(Thread *that) in Aufgabe 5 verwendet werden.

\dotfile "a6.dot" "Klassenübersicht für Aufgabe 6"

# Bearbeitung #
## Teil A ##
Beginnt am besten mit der Implementierung der Semaphoren. Dazu müssen
zunächst die Klassen Waitingroom, Semaphore, Thread, Scheduler
und Guarded_Semaphore implementiert werden. Mit Hilfe von
Semaphorvariablen solltet ihr nun verhindern können, dass sich eure
Anwendungsthreads bei der Bildschirmausgabe gegenseitig behindern.

## Teil B ##
Im nächsten Schritt könnt ihr dann die Klasse Keyboard um die
getkey()-Methode erweitern, durch die Threads nun mit Hilfe einer
Semaphore beim Lesen von der Tastatur blockieren, falls keine Taste
gedrückt wurde. Erweitert euer Programm dahingehend, dass einer der
Threads Tastaturabfragen vornimmt (und in irgendeiner sichtbaren Weise
darauf reagiert).

## Teil C ##
Anschließend können die Klassen Bell und Bellringer implementiert und
getestet werden. Von Bellringer sollte eine globale Instanz \b bellringer
angelegt werden. Wenn klar ist, dass insbesondere der Bellringer
funktioniert, wie er sollte, könnt ihr die Klassen Bell und
Guarded_Bell erstellen. Damit lassen sich nun sehr einfach periodische
Threads erstellen, indem ein Thread sich immer wieder für ein paar
Millisekunden schlafen legt und dann z. B. eine Ausgabe macht. Auch dafür
sollt ihr ein geeignetes Beispiel vorbereiten.

## Teil D ##
Wenn alle vorhergenden Klassen implementiert und getestet wurden, kann man
nun die Idle-Problematik angehen.
Dies sollte auch getestet werden, indem man in der
Testapplikation regelmäßig dafür sorgt, dass zuwenig (in MPStuBS) oder gar
keine Threads zum Ausführen vorhanden sind.

## Teil E (optional) ##

 Hat man die Idle Problematik gelöst, ist es relativ einfach eine
 Implementierung zu bauen, bei der der Timer die CPUs nicht
 beständig wieder aus dem Idle holt. Dies ist unter Energieaspekten
 wünschenswert. Um dies zu erreichen implementiert man noch die
 Watch::block() und Watch::unblock() Funktionen und reichert die
 Idleloop um diese an. Es ist wichtig zu beachten, dass man den
 Timer auf CPU0 nur dann ausschaltet, wenn keine Bell mehr auf
 Abarbeitung wartet.
