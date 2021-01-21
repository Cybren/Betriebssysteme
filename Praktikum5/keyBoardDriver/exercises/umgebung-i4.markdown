\page umgebung Entwicklungsumgebung zu BS

Für die Bearbeitung der Rechnerübungsaufgaben (und damit für die
Entwicklung von OOStuBS und MPStuBS) sind alle benötigten Werkzeuge im
CIP installiert. Ihr könnt die Aufgaben natürlich auch zu Hause
bearbeiten, wir empfehlen hierzu den Einsatz von Linux.  Weiter unten
finden sich einige Hinweise, wie ihr euren Linux-Rechner entsprechend
konfigurieren könnt.

\attention Wer die Software bei sich zu Hause installieren möchte, trägt
  natürlich die volle Verantwortung für eventuelle Probleme.
  Fehlgeschlagene Installationen werden von uns auch nicht als
  Entschuldigung für verspätete Programmabgaben akzeptiert.

Da sich bei der Betriebssystementwicklung ab und zu auch Fehler
einschleichen können, müsst ihr eure Lösungen testen, bevor ihr sie
abgebt. Wir benutzen hierzu einen Emulator (`QEMU`) und einen echten
Multicore-Rechner im Rechnerübungsraum. Bei der Abgabe benutzen wir
immer den echten PC, um eure Lösung zu kontrollieren. Ihr solltet
deshalb immer auch mit dem echten PC testen, er ist die
Referenzplattform!

Voraussetzungen zum Kompilieren von OOStuBS bzw. MPStuBS im CIP-Pool und zu Hause
---------------------------------------------------------------------------------

Zum Kompilieren wird wie im Makefile vorgegeben `g++` verwendet, zum
Assemblieren des Startup-Codes und der hardwarenahen Teilprogramme der
[Netwide Assembler](http://nasm.sourceforge.net/) (`nasm`). Der
x86-Emulator `QEMU` eignet sich zum vorläufigen Testen und, durch einen
eingebauten GDB-Stub, auch zum Debuggen mit dem `GNU Debugger`. Im
CIP-Pool ist die entsprechende Umgebung vorhanden bzw. unter
`/proj/i4bs/` gemountet; wer das Ganze zu Hause machen will, muss sich
die genannte Software entsprechend einrichten. Bei Problemen könnt ihr
uns gerne fragen.

Kompilieren von OOStuBS bzw. MPStuBS
------------------------------------

-   Zu Aufgabe 1 gibt es je eine Vorgabe für OOStuBS
    (https://gitlab.cs.fau.de/i4/bs/oostubs) und MPStuBS
    (https://gitlab.cs.fau.de/i4/bs/mpstubs). Die Vorgaben sind Git
    Repositories auf dem [CIP Gitlab](https://gitlab.cs.fau.de), in
    die wir die Vorgaben für die einzelnen Aufgaben sukkessive
    einchecken.

    Um die Änderungen mit eurem Übungspartner zu teilen, könnt ihr
    euch einen Account auf dem Gitlab anlegen und ihm dort Zugriffsrechte gewähren.

    \attention Bei [gitlab.cs.fau.de](https://gitlab.cs.fau.de) muss euer Benutzername der CIP Login Name sein.

        ad42resu@faui0sr0:~> git clone https://gitlab.cs.fau.de/i4/bs/oostubs.git
        ad42resu@faui0sr0:~> find oostubs
        oostubs/user/
        oostubs/user/app1/
        oostubs/user/app1/appl.cc
        [...]

-   Für die weiteren Aufgaben werden wir die die Vorgabenrepositories
    erweitern, sodass ihr euch nur die Änderungen in euer Repository
    ziehen müsst. Diese geänderten Vorgaben enthalten für die jeweilige Aufgabe
    neu hinzukommenden Dateien.

-   Alle Vorgaben, die ihr von uns erhaltet, lassen sich korrekt
    übersetzen, enthalten aber nur unvollständigen Code. Ihr müsst also
    Code in den unter Aufgaben beschriebenen Funktionen und Klassen
    vervollständigen.

-   Das eigentliche Kompilieren von OOStuBS erfolgt durch den Aufruf von
    `make` im Lösungsverzeichnis. Alle `.cc`- und `.asm`-Dateien im
    Lösungsverzeichnis werden daraufhin mit den entsprechenden Tools
    (Compiler bzw. Assembler) übersetzt und als bootbares Systemimage
    zusammengebunden. Anschließend stehen euch die Befehle
    `make {kvm,qemu,netboot}{,-gdb,-ddd}{,-noopt,-verbose}` zum Testen
    und Debuggen zur Verfügung (mehr dazu im nächsten Abschnitt).

Testen und Debuggen von OOStuBS
-------------------------------

-   Als schnellsten und einfachsten Test eurer Implementierung könnt ihr
    mit **make kvm** euer Systemimage in QEMU mit
    Hardware-Virtualisierung ausführen:

        ad42resu@faui04a:~/oostubs> make kvm

    Dabei wird QEMU standardmäßig so konfiguriert, dass er ein System
    mit vier Prozessoren emuliert. Für die Entwicklung von OOStuBS stört
    dies nicht weiter, da die zusätzlichen CPUs ohne weiteres Zutun
    einfach "links liegen" gelassen werden. Für die MPStuBS-Bastler
    gilt: durch den KVM-Modus wird euer System echt parallel auf
    mehreren Kernen ausgeführt. Dieser Test kommt daher im Hinblick auf
    Race-Conditions und fehlerhafter Synchronisation dem Test auf der
    echten Hardware schon relativ nahe.

-   Zur leichteren Fehlersuche kann die Hardware-Virtualisierung auch
    deaktiviert werden, indem man stattdessen den Befehl **make qemu**
    verwendet. In diesem Modus wird das Gastsystem lediglich
    pseudo-parallel emuliert, was bei schwerwiegenderen Bugs die Suche
    erleichtert, aber andererseits auch vorhandene maskieren kann, die
    sonst nur mit `make kvm` oder auf echter Hardware auftreten.

-   Wer bei der Fehlersuche mit einfachem "`printf`-Debugging" nicht
    weiterkommt, der kann den in QEMU integrierten GDB-Stub verwenden,
    um sich mit einem Debugger (`gdb` oder `ddd`) zu der Emulation zu
    verbinden. Auf diese Weise könnt ihr euren Betriebssystemcode bequem
    schrittweise ausführen, um den Grund etwaiger Abstürze oder
    ungewünschten Verhaltens herauszufinden. Dafür stellen wir im
    Makefile die Targets **qemu-gdb** und **qemu-ddd** bereit:

        ad42resu@faui04a:~/oostubs> make qemu-gdb

    In dieser Konfiguration wartet der GDB-Stub im Emulator auf eine
    Socket-Verbindung, über die sich ein `gdb` oder `ddd` mit dem
    Emulator verbinden kann, um das System zu debuggen. Der Start des
    Debuggers wird bereits im Makefile erledigt, so dass der
    `gdb`-Prompt unmittelbar nach dem Start von QEMU im Terminal
    erscheint.

    Eine knappe Referenz der GDB-Funktionen könnt ihr
    [hier](http://users.ece.utexas.edu/~adnan/gdb-refcard.pdf) finden.
    Wollt ihr detailierte Hinweise, wie ein bestimmter GDB-Befehl zu
    verwenden ist, so könnt ihr die in GDB eingebaute Hilfefunktion
    nutzen:

        (gdb) help <Befehlsname>

    \attention Da durch den Emulator QEMU bei Verwendung des GDB-Stubs das
    Betriebssystem pausiert wird, darf man im GDB/DDD die
    Programmausführung nicht mit `run` neu anstoßen, sondern muss sie
    stattdessen mit `continue` fortführen.

    Für einen schnelleren Überblick der Register- und Stackinhalte
    empfiehlt sich außerdem, diese [gdbinit](../gdbinit)-Datei unter dem
    Namen `.gdbinit` im eigenen Home-Verzeichnis abzulegen. Diverse
    Ansichtsoptionen am Anfang der Datei können ganz nach dem eigenen
    Geschmack geändert werden.

-   Zum Testen auf dem Referenz-PC im Rechnerübungsraum gibt es das
    Makefiletarget **netboot**:

        ad42resu@faui04a:~/oostubs> make netboot

    Dieses kopiert euer gebautes Systemimage auf einen TFTP-Server, so
    dass der Test-PC mit Hilfe des Netzwerkbootloaders PXELinux Zugriff
    darauf erhält. Die Test-PCs booten beim Einschalten automatisch aus
    dem Netz. Im dann erscheinenden Boot-Menü müsst ihr nur noch das
    aktuelel Semester und anschließend den Eintrag mit euerem Login-Namen
    auswählen, um euren OOStuBS/MPStuBS-Kernel zu booten.

    Falls ihr gerade nicht im CIP seid, könnt ihr auch über die
    [Weboberfläche](https://i4stubs.informatik.uni-erlangen.de/)
    mit dem Benutzer und Passwort aus der Tafelübung auf die Rechner
    zugreifen. Alternativ könnt ihr auch das Tool `/proj/i4bs/tools/stubs`
    verwenden, um die Testrechner zu steuern.

    \attention Versucht den Zugriff auf die Testrechner mit euren Kommilitonen
    abzustimmen, damit niemand beim testen unterbrochen wird.
    Während den Rechnerübungen sollte die Benutzungsdauer an der Hardware
    möglichst kurz sein.

-   Hinweis zu Compileroptimierungen: Das Standardverhalten des
    vorgegebenen Buildsystems ist, das Optimierungslevel -O3 zu
    verwenden. Bei hartnäckigen Bugs kann der daraus entstehende
    Maschinencode das Debugging deutlich erschweren. F¨r diese Fälle
    gibt es alle oben genannten Makefile-Targets auch als Variante mit
    dem Suffix **-noopt**, welches die Compileroptimierungen
    deaktiviert. GDB-Debugging ohne Optimierungen ist beispielsweise mit
    diesem Aufruf möglich:

        ad42resu@faui04a:~/oostubs> make qemu-gdb-noopt

Quellcodeverwaltung in einem gemeinsamen Git für die ganze Gruppe
----------------------------------------------------------------------------

Wie bereits erwähnt, ist die Vorlage zu OOStuBS bzw. MPStuBS auf
[gitlab.cs.fau.de](https://gitlab.cs.fau.de) zu finden. Dort habt ihr
auch die Möglichkeit ein eigenes Repository anzulegen und eurem
Übungspartner Zugriff darauf zu gewähren.

Eine kurze Übersicht der Git Befehle findet ihr
[hier](http://www.git-tower.com/blog/git-cheat-sheet-de). Als
tieferen Einstieg in die verteilte Quellcodeverwaltung empfehlen wir
das [Pro Git](https://git-scm.com/book/de/v1) Buch, welches als
Creative Commons verfügbar ist.
