Aufgabe 4: Threadumschaltung für OOStuBS/MPStuBS {#aufgabe4}
============================================================

# Lernziele #
- Auffrischen der Assemblerkenntnisse (siehe auch \ref assembler)
- Verständnis der Abläufe beim Threadwechsel
- Unterscheidung von aktiven und passiven Objekten

# Aufgabenbeschreibung #
Es soll eine einfache Threadverwaltung implementiert werden, bei der die
Benutzerthreads die Prozessorabgabe im Sinne des Koroutinenkonzepts selbst
regeln.

Dazu müssen einige Funktionen zum Zugriff auf die Struktur ::toc, die Klassen
Dispatcher, Thread und Scheduler, sowie die Funktion Dispatcher::kickoff()
implementiert werden. Außerdem soll OOStuBS/MPStuBS nun spätestens jetzt mit
Application eine Anwendung erhalten.

Um die Threadumschaltung überall in OOStuBS/MPStuBS ansprechen zu können,
soll in Aufgabenteil B eine globale Instanz des Dispatchers angelegt und in
Teil C durch eine globale Instanz scheduler des Schedulers ersetzt werden.

\dotfile "a4.dot" "Klassenübersicht für Aufgabe 4"

Die Funktionsfähigkeit der genannten Klassen soll mit Hilfe eines
aussagefähigen Testprogramms gezeigt werden. Dazu soll in main.cc nun
zusätzlich eine globale Instanz scheduler der Klasse Scheduler angelegt und
mit mehreren Benutzerthreads gefüllt werden, welche sich ähnlich zu den
bisherigen Testprogrammen jeweils auf einer eigenen Bildschirmposition
bemerkbar machen. Testet auch die Methoden Scheduler::exit() und
Scheduler::kill(Thread *item).

# Implementierungshinweise #

Zum Testen empfiehlt es sich, die Gesamtaufgabe in drei Teile zu teilen und
mit Teil B und C erst zu beginnen, wenn Teil A (bzw. B) fertig
implementiert und ausreichend getestet wurden. Für diese Aufgabe könnt ihr
die Interrupts wieder deaktivieren. Es laufen also nur die Threads auf
den/der %CPU(s) und ihr braucht euch so nicht um Synchronisation zwischen
Threadkontrollfluss und Interrupthandlern zu kümmern. Interrupts kommen
erst wieder in Aufgabe 5 ins Spiel.

## Teil A ##
Hier wird der Threadwechsel realisiert. Es müssen also zunächst nur die
Zugriffsfunktionen auf die Struktur ::toc und die Klasse Thread
implementiert werden. Zum Testen der Lösung sollten mehrere Threads
angelegt werden, welche jeweils nach einigen Anweisungen den Prozessor an
den nächsten Thread abgeben.

## Teil B ##
Als nächstes kann der Dispatcher implementiert werden. Im Testprogramm
sollte der Threadwechsel nun über den Dispatcher laufen können.

## Teil C ##
Zum Schluss sollte der Scheduler hinzugefügt werden.
Im Testprogramm müssen die Anwendung und Benutzerthreads nun beim
Scheduler angemeldet werden. Dafür brauchen sie sich nicht länger
gegenseitig zu kennen, denn die Auswahl des nächsten Threads kann nun der
Scheduler übernehmen.

## MPStuBS ##
Die Implementierung für MPStuBS ist analog zur Uniprozessorimplementierung
aufgebaut. Die Threads werden in einer einzigen Bereitliste verwaltet.
Jedoch kann es vorkommen, dass verschiedene Prozessoren zur gleichen Zeit
auf die Datenstruktur des Schedulers zugreifen. Aufrufe des Schedulers
müssen deswegen in MPStuBS auch bei kooperativem Scheduling synchronisiert
werden. Insbesondere ist darauf zu achten, dass ein auf der aktuellen %CPU
laufender Thread nicht vorzeitig für die Ausführung auf einer anderen %CPU
bereitgestellt werden darf.

Grundsätzlich ist es für MPStuBS sinnvoll die Implementierung schrittweise
wie oben beschrieben durchzuführen, dabei das Scheduling jedoch zuerst nur
auf einer %CPU zu starten. Erst wenn dies problemlos funktioniert, sollte
das Scheduling auch auf den Applikationsprozessoren gestartet werden. Dies
vereinfacht das Debugging enorm.

Es darf bei der Implementierung angenommen werden, dass immer genug
Threads vorhanden sind, um alle Prozessoren zu beschäftigen. Diese
Voraussetzung müsst ihr dann aber natürlich auch in euerem Testprogramm
erfüllen. Es empfiehlt sich dabei, den Threadwechsel intensiv mit
verschieden vielen Threads zu testen.
