Aufgabe 5: Zeitscheibenscheduling für OOStuBS/MPStuBS {#aufgabe5}
=================================================================

# Lernziele #
- Schutz kritischer Abschnitte im Betriebssystem mit Hilfe des Guard
- Implementierung präemptiven Schedulings durch Timer-Interrupts

# Aufgabenbeschreibung #
Die Synchronisation der Aktivitäten innerhalb von OOStuBS/MPStuBS ist in Aufgabe 3
auf die Verwendung des Pro-/Epilogmodells umgestellt worden. Nun soll eine
grobgranulare Locking-Strategie verwendet und eine
Systemaufrufschnittstelle definiert werden. Der Scheduler soll - ausgelöst
durch den Timer-Interrupt - Threads verdrängen können.

Hierzu müssen die Klassen Guarded_Scheduler, Watch und (in MPStuBS)
Assassin implementiert sowie die Klasse LAPIC um die Implementierung von
LAPIC::setTimer erweitert werden.

Um aus Benutzerprogrammen heraus die Methoden des Schedulers
aufrufen zu können, darf die globale Variable \b scheduler nicht direkt
benutzt werden. Stattdessen soll die Klasse Guarded_Scheduler mit Hilfe des
\b guard die Zugriffe auf den globalen Scheduler schützen.

\dotfile "a5.dot" "Klassenübersicht für Aufgabe 5"

# MPStuBS #
In MPStuBS gab es schon in Aufgabe 4 die Problematik, dass die Methoden des
Schedulers von mehreren Prozessoren aus gleichzeitig aufgerufen werden
konnten, und deswegen die Zugriffe auf die Bereitliste synchronisiert
werden mussten.  Da die Betriebssystemfunktionen nun in der
Syscall-Schnittstelle mit dem Guard synchronisiert werden sollen, müssen
die expliziten Synchronisationsaufrufe aus Aufgabe 4 entfernt werden.

Analog zum Scheduling soll nun auch die Implementierung von
Scheduler::kill(Thread*) präemptiv erfolgen. Sollte der zu beendende
Thread gerade auf einem anderen Prozessor laufen, dann muss dieser mit
Hilfe eines Interprozessorinterrupts (IPIs) unterbrochen und zur Abgabe des
Prozessors gezwungen werden. Dazu stellen wir in der Klasse APICSystem
die Methode APICSystem::sendCustomIPI() bereit, die mit Hilfe des Local
APICs Interrupts an andere CPUs schicken soll. Die Interruptbehandlung
für den IPI soll in der Klasse Assassin implementiert werden.

# Hilfestellung #
- \subpage apictimer
