\page apic APIC-Architektur: Externe Interrupts in modernen PCs

Moderne PCs verwenden nicht mehr den [PIC8259A](https://de.wikipedia.org/wiki/Intel_8259),
um externe Interrupts an die CPU anzubinden. Stattdessen kommt der APIC zum
Einsatz. Die APIC-Architektur besteht dabei aus mehreren Chips:

![Schema der APIC Architektur auf Intel IA-32/AMD64](img/apic.svg)

- Jede CPU besitzt einen *Local APIC*, der Interruptanforderungen von außen
  entgegennimmt und an den %CPU-Kern weiterleitet. In Multiprozessorsystemen
  ist es darüberhinaus auch möglich anderen CPUs einen Interprozessorinterrupt
  zu schicken und so zwischen den einzelnen CPUs zu kommunizieren.
- Externe Interruptquellen wie z.B. Tastatur, Maus, Timer etc. sind an den
  sog. *IO-APIC* angeschlossen. Dieser besitzt dafür 24 Eingänge. Alle
  Interrupteingangspins am IO-APIC sind grundsätzlich gleichberechtigt. Der
  IO-APIC reagiert in FIFO-Reihenfolge auf mehrere externe Interrupts.

Alle APICs eines Systems (egal ob local oder IO-APIC) kommunizieren je nach
System mittels des Systembusses oder über einen dedizierten APIC-Bus. Für
den Systemprogrammierer (also euch 😉) spielt das jedoch keine Rolle, da sich
beide Implementierungen auf Ebene der Schnittstelle gleich verhalten.

Die Zuordnung von externen Interrupts zu Interruptvektoren in der %CPU ist
im IO-APIC frei programmierbar. Dazu besitzt der IO-APIC eine Tabelle
(Redirection Table), in der für jeden Eingangspin der in der %CPU
auszulösende Interruptvektor konfiguriert werden kann. Darüberhinaus ist es
hier möglich die Art des externen Interrupts (edge- oder leveltriggered) zu
konfigurieren und ihn auszumaskieren. Auch die Menge aller zur
Interruptbearbeitung in Frage kommenden Prozessoren kann hier konfiguriert
werden.

Stellt der IO-APIC nun fest, dass auf einem der Interrupteingänge eine
Interruptanforderung anliegt, so wird anhand der jeweiligen Konfiguration
entschieden, an welche %CPU der konfigurierte Interruptvektor gesendet
werden soll. Dieser wird dann durch den Local APIC der jeweiligen %CPU an
den eigentlichen Prozessorkern weitergeleitet, wo dann die
Unterbrechungsbehandlung gestartet wird. Nach Durchführung der Behandlung
muss dem Local APIC dies mitgeteilt werden. Erst wenn dies geschehen ist,
wird man weitere Interruptanforderungen dieses Typs erhalten.

Im Gegensatz um IO-APIC nimmt der Local APIC nun eine Priorisierung vor.
Die Priorität eines Interrupts hängt dabei von seiner Vektornummer ab:

    priority = vector / 16

Da die Vektoren `0` bis `31` reserviert sind, stehen die Prioritäten `2` bis
`15` der Anwendung zur Verfügung. Die ersten 32 Vektoren (`0` bis `31`) sind
für Ausnahmesituationen vergeben, welche lokal auf der CPU erzeugt
werden. Dies kann beispielsweise eine `Invalid Opcode`-Ausnahme oder
`division by zero`-Ausnahme sein.

## Software

### Ignorieren eines Interrupts

CPU-seitig kann dafür gesorgt werden, daß das laufende Programm nicht
durch Interrupts unterbrochen wird. Dazu wird mit der Assembleranweisung \b
cli das Interrupt-Bit im `EFLAGS`-Register gelöscht. Der Prozessor wird nun
auf weitere, durch den Local APIC zugestellte Interrupts nicht mehr
reagieren. Mit dem Befehl \b sti werden die Interrupts im Prozessorkern
wieder zugelassen.

Externe Interrupts können auch selektiv unterdrückt werden. Dazu muß der
IO-APIC programmiert werden. Da das APIC Subsystem eine relativ neue
PC-Komponente ist, wird die Programmierung nicht über die \b in und \b out
Befehle der x86 %CPU vorgenommen. Stattdessen werden die Register des Local
und IO-APICs memorymapped angesprochen.

###  Software zur Interruptbehandlung

Wenn eine Interruptanforderung eines externen Gerätes ankommt dann
verzweigt der Prozessor automatisch zur entsprechenden
Interruptbehandlungsroutine.

Deren Adresse wird einer Interruptdeskriptortabelle (IDT) entnommen, wobei
die im IO-APIC konfigurierte Vektornummer als Index in die Tabelle dient.
Die Vektornummer wird dabei vom IO-APIC über den APIC-Bus oder den
Systembus an einen Local APIC geschickt, der dann dafür sorgt, dass die
%CPU unterbrochen wird.  Während beim 8086 Prozessor die Lage der
Interruptdeskriptortabelle noch fest von der Hardware vorgegeben war, wird
beim 80386 ihr Anfang und ihre Größe durch das IDT-Register beschrieben.

Die Interruptdeskriptortabelle kann maximal 256 Interruptgate-Deskriptoren
enthalten, von denen es drei verschiedene Typen gibt:

####  Task-Gate

Der Interruptgate-Deskriptor zeigt auf einen Task, einen hardwaremäßig
unterstützten Prozeß. Wenn der entsprechende Interrupt eintritt, führt der
Prozessor automatisch einen Taskwechsel zu dem angegebenen Interrupt-Task
durch.

#### Interrupt-Gate
Der Interruptgate-Deskriptor zeigt auf eine Prozedur, die als
Interruptbehandlungsroutine ohne vorherigen Taskwechsel aufgerufen wird.
Während der Behandlungsroutine werden die Interrupts von der %CPU
automatisch deaktiviert.

#### Trap-Gate
Der Trapgate-Deskriptor zeigt auf eine Prozedur, die als
Trapbehandlungsroutine ohne vorherigen Taskwechsel aufgerufen wird. Die
Interrupts bleiben hier jedoch im Gegensatz zum Interruptgate aktiv.

Bevor der Prozessor infolge eines Interrupts oder Traps die angegebene
Behandlungsroutine aufruft, legt er den aktuellen Inhalt des `EFLAGS`
Registers auf den Stack ab. Dies ermöglicht es ihm, nun das
Interrupt-Enable-Flag im `EFLAGS`-Register zu löschen und auf diese Weise die
geschachtelte Behandlung weiterer Interrupts zu verhindern. Wie bei einem
normalen Funktionsaufruf wird dann noch die Rücksprungadresse (Inhalt von
Code-Segment und Instruction Pointer) auf dem Stack gesichert, bevor die
Behandlungsroutine begonnen wird.
Bei manchen Ausnahmen legt der Prozessor zusätzlich einen Fehlercode auf
dem Stack ab.

Wurde die Unterbrechung durch einen Interrupt ausgelöst, so besteht eine
Aufgabe der Interruptbehandlungsroutine darin, dem Local APIC mitzuteilen, dass
der Interrupt behandelt wurde. Anderenfalls wird der APIC nämlich keine weiteren
Interrupts desselben Gerätes weiterleiten. Das Senden des
Interrupt-Acknowledge-Signals erfolgt dabei durch schreiben in ein
spezielles Register des Local APICs.

Mit dem \b iret Befehl wird die Unterbrechungsbehandlung abgeschlossen. Der
Prozessor holt die Rücksprungadresse vom Stack, stellt den Inhalt des
`EFLAGS` Registers wieder her und kehrt zu der unterbrochenen Funktion
zurück. Dadurch, daß auch die `EFLAGS` wieder hergestellt werden, werden
spätestens jetzt die Interrupts CPU-seitig wieder zugelassen.

### Zugriff auf die Register des Local APICs bzw. des IO-APICs.

Beide Komponenten, Local APIC und IO-APIC, werden grundsätzlich über
memory-mapped IO angesprochen. In der Art und Weise wie jedoch die internen
Register angesprochen werden unterscheiden sich jedoch beide grundsätzlich.
Beim Local APIC sind alle internen Register in den Adressraum des
Prozessors eingeblendet. Die internen Register des IO-APICs sind hingegen
nicht direkt zugreifbar, sondern werden über die die beiden Register
`IOREGSEL` und `IOWIN` angesprochen. Dazu schreibt man zuerst die Adresse des
gewünschten internen Registers in `IOREGSEL`. Aus `IOWIN` kann man nun den Wert
des internen Registers auslesen oder neu schreiben.

Die Register des IO-APICs und ihre Beschreibung findet ihr in
machine/ioapic_registers.h; für den Local APIC befinden sich diese in
machine/lapic_registers.h

Die Lowlevelprogrammierung des Local APICs haben wir für euch schon
erledigt. Ihr könnt also einfach die Klasse LAPIC verwenden, wenn ihr
Funktionen des Local APICs benötigt. Die Zugriffsfunktionen für den
IO-APIC hingegen müssen von euch noch programmiert werden, indem ihr die
Klasse IOAPIC vervollständigt.

## Literatur
  - [Intel, IO-APIC Manual/Datasheet](intel_ioapic.pdf)
  - [Intel 64 and IA-32 Architectures Software Developer's Manual; Volume 3A: System Programming Guide, Part 1, Kapitel 6 & Kapitel 10](intel_manual_2017-03.pdf)
