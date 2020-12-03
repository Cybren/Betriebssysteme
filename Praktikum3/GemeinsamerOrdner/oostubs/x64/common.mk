# --------------------------------------------------------------------------
# Einstellungen in Abhaengigkeit vom Generierungssystem:
#
#    ASMOBJFORMAT: Ausgabeformat fuer den Assembler. Das Format muss dem
#                  jeweiligen Format des verwendeten C++ Compilers angepasst
#                  werde, damit gemischte Objektdateien gelinkt werden koennen.
#    BUILDDIR:     Verzeichnispräfix, in dem die bei der Übersetzung generierten
#                  Dateien abgelegt werden sollen.
#    ISODIR:       Verzeichnispräfix, in dem die Daten für das bootfähige
#                  Medium angelegt werden sollen
#    NOOPTTAG:     Endung, welche an die Verzeichnisse für Objekt- und
#                  Abhängigkeitsdateien bei Debugversionen angehängt wird
#    VERBOSETAG:   Endung ähnlich NOOPTTAG, nur für Builds mit Verboseausgaben
#    ASM:          Zu benutzender Assembler
#    CC/CXX:       Zu benutzender C/C++-Compiler
#    AR:           Zu benutzender Archivierer
#    CFLAGS:       Flags fuer den C-Compileraufruf
#    CXXFLAGS:     Flags fuer den C++-Compileraufruf
#    LD:           Zu benutzender Linker
#    LDFLAGS:      Flags fuer den Linkeraufruf
#    LDLIBS:       Dazuzulinkende Bibliotheken
#    LDHEAD:       Am Anfang zu linkende Dateien
#    LDTAIL:       Am Ende zu linkende Dateien

VERBOSE = @
ASMOBJFORMAT = elf64
BUILDDIR = .build
NOOPTTAG = -noopt
VERBOSETAG = -verbose
SOLUTIONDIR = /proj/i4bs/solution
SOLUTIONPREFIX = musterloesung-u
ASM = nasm
CXX = g++
MKISO = grub-mkrescue
QEMU = qemu-system-x86_64

PROJECT="OOStuBS"

OPTFLAGS = -O3 -fomit-frame-pointer
ARCHFLAGS = -m64
CXXFLAGS = -std=c++11 $(ARCHFLAGS) -ffreestanding -fno-pic -nodefaultlibs -nostdlib -nostdinc -I. $(OPTFLAGS) -Wall -Wextra -Werror -Wno-error=unused-parameter -mno-mmx -mno-sse -Wno-non-virtual-dtor -fno-rtti -fno-exceptions -Wno-write-strings -fno-stack-protector -mno-red-zone -g -gdwarf-2
ifneq (,$(findstring clang++,$(CXX)))
	COMPILER := CLANG
	CXXFLAGS += -Wno-error=unused-private-field -Wno-implicit-exception-spec-mismatch -Wno-error=unused-const-variable -Wno-unused-command-line-argument -Wno-unused-const-variable
else ifneq (,$(findstring g++,$(CXX)))
	COMPILER := GCC
	CXXFLAGS += -fno-tree-loop-distribute-patterns -no-pie -nostartfiles -Wstack-usage=1024 -Wno-error=stack-usage=
else
	COMPILER :=
endif
ASMFLAGS = -f $(ASMOBJFORMAT)
LIBGCC += $(shell $(CXX) $(CXXFLAGS) -print-libgcc-file-name )
CRTBEGIN_OBJECT = $(shell $(CXX) $(CXXFLAGS) --print-file-name=crtbegin.o)
CRTEND_OBJECT = $(shell $(CXX) $(CXXFLAGS) --print-file-name=crtend.o)
AR = ar
QEMUCPUS = 4
QEMUFLAGS = -k en-us -serial pty -d guest_errors
QEMUDBGFLAGS = -no-shutdown -no-reboot
KVMFLAGS = -enable-kvm -cpu host $(QEMUFLAGS)
INITRD = /dev/null

NETBOOTDIR = /proj/i4bs/student
NETBOOTSSH = faui04a.cs.fau.de

ISODIR = $(BUILDDIR)-iso
ISOGRUBCFG = boot/grub/grub.cfg
ISOKERNEL = boot/kernel
ISOINITRD = initrd
GRUBTIMEOUT = 2
GRUBBIN = /usr/lib/grub/i386-pc
DD = dd
XORRISO = xorriso
ifeq (,$(wildcard $(GRUBBIN)))
	GRUBBIN = /proj/i4bs/tools/grub-i386-pc
endif

# Namen der Unterverzeichnisse mit den Quelltexten
VPATH = $(sort $(dir $(CC_SOURCES) $(ASM_SOURCES) $(CPP_SOURCES)))

# -------------------------------------------------------------------------
# Listen mit den Objektdateien, die beim Kompilieren entstehen:
# (Die Eingabevariablen werden vom eigentlichen Makefile zwar erst spaeter
# definiert, sind aber zum Auswertezeitpunkt *dieser* Variablen schon
# verfuegbar.)
CRTI_OBJECT = $(addprefix $(BUILDDIR)/,$(addsuffix .o,$(CRTI_SOURCE)))
CRTN_OBJECT = $(addprefix $(BUILDDIR)/,$(addsuffix .o,$(CRTN_SOURCE)))
CC_OBJECTS = $(addprefix $(BUILDDIR)/,$(CC_SOURCES:.cc=.o))
DEP_FILES = $(addprefix $(BUILDDIR)/,$(CC_SOURCES:.cc=.d) $(addsuffix .d,$(ASM_SOURCES)))
ASM_OBJECTS = $(addprefix $(BUILDDIR)/,$(addsuffix .o,$(filter-out $(CRTI_SOURCE) $(CRTN_SOURCE),$(ASM_SOURCES))))

# --------------------------------------------------------------------------
# Standardrezept zur Erzeugung der Abhaengigkeitsdateien
$(BUILDDIR)/%.d : %.cc $(MAKEFILE_LIST)
	@echo "DEP		$<"
	@mkdir -p $(@D)
	$(VERBOSE) $(CXX) $(CXXFLAGS) -MM -MT $(BUILDDIR)/$*.o -MF $@ $<

$(BUILDDIR)/%.asm.d : %.asm $(MAKEFILE_LIST)
	@echo "DEP		$<"
	@mkdir -p $(@D)
	$(VERBOSE) $(ASM) $(ASMFLAGS) -MT $(BUILDDIR)/$*.asm.o -MF $@ $<

# --------------------------------------------------------------------------
# Standardrezept zur Erzeugung von Objektdateien
$(BUILDDIR)/%.o : %.cc $(MAKEFILE_LIST)
	@echo "CXX		$<"
	@mkdir -p $(@D)
	$(VERBOSE) $(CXX) -c $(CXXFLAGS) -o $@ $<

$(BUILDDIR)/%.asm.o : %.asm $(MAKEFILE_LIST)
	@echo "ASM		$<"
	@mkdir -p $(@D)
	$(VERBOSE) $(ASM) $(ASMFLAGS) -o $@ $<

# --------------------------------------------------------------------------
# Standardrezept 'clean' loescht das generierte System, die Objektdateien und
# die Abhaengigkeitsdateien

clean:
	@echo "RM		$(BUILDDIR)"
	$(VERBOSE) rm -rf "$(BUILDDIR)"
	$(VERBOSE) rm -rf "$(BUILDDIR)$(NOOPTTAG)"
	$(VERBOSE) rm -rf "$(BUILDDIR)$(VERBOSETAG)"
	@echo "RM		$(ISODIR)"
	$(VERBOSE) rm -rf "$(ISODIR)"
	$(VERBOSE) rm -rf "$(ISODIR)$(NOOPTTAG)"
	$(VERBOSE) rm -rf "$(ISODIR)$(VERBOSETAG)"

# --------------------------------------------------------------------------
# Rezept fuer rekursiven Aufruf mit deaktivierten Optimierungen
%-noopt:
	make BUILDDIR="$(BUILDDIR)$(NOOPTTAG)" ISODIR="$(ISODIR)$(NOOPTTAG)" OPTFLAGS="-O0" $*

# --------------------------------------------------------------------------
# Rezept fuer rekursiven Aufruf mit erweiterten Ausgaben
%-verbose:
	make BUILDDIR="$(BUILDDIR)$(VERBOSETAG)" ISODIR="$(ISODIR)$(VERBOSETAG)" OPTFLAGS="-DVERBOSE" $*

# --------------------------------------------------------------------------
# Standardrezepte zum Ausfuehren und Debuggen
# --------------------------------------------------------------------------

qemu: all
	$(QEMU) -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) $(QEMUFLAGS)

# --------------------------------------------------------------------------
# 'qemu-gdb' ruft den qemu-Emulator mit aktiviertem GDB-Stub mit dem System
# auf, sodass es per GDB inspiziert werden kann.
qemu-gdb: all
	gdb $(KERNEL) \
		-ex "set arch i386:x86-64" \
		-ex "target remote | exec $(QEMU) -gdb stdio -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) -S $(QEMUFLAGS) $(QEMUDBGFLAGS)"

# --------------------------------------------------------------------------
# 'kvm-gdb' ruft den hardwarebeschleunigten qemu-Emulator mit aktiviertem
# GDB-Stub mit dem System auf, sodass es per GDB inspiziert werden kann.
kvm-gdb: all
	gdb $(KERNEL) \
		-ex "set arch i386:x86-64" \
		-ex "target remote | exec $(QEMU) -gdb stdio -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) -S $(KVMFLAGS) $(QEMUDBGFLAGS)"

# --------------------------------------------------------------------------
# 'kvm' startet euer System in einem QEMU mit kvm-Unterstützung, um auch echt
# parallel arbeitende virtuelle CPUs nutzen zu können. Maximal sind zwei
# virtuelle CPUs möglich. Dieses Kommando ist lediglich ein Wrapper um kvm, und
# funktioniert nur im CIP-Pool, da es dort aus Sicherheitsgründen nicht möglich
# ist kvm direkt selbst zu starten.  Wenn ihr zuhause mit kvm testen wollt,
# müsst ihr das selbst aufrufen, was genauso funktioniert wie der Aufruf von
# QEMU in den beiden vorherigen Makefiletargets.
kvm: all
	$(QEMU) -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) $(KVMFLAGS)

# --------------------------------------------------------------------------
# 'iso' erstellt ein bootfähige Abbild, welches auf ein externes Medium (wie
# CD oder [etwas moderner] USB übertragen werden kann. Dazu wird neben eurem
# Kernel auch der Bootloader Grub ( https://www.gnu.org/software/grub/ )
# installiert.
# Mit dem Target 'qemu-iso' kann dieses Medium getestet werden.
# Sofern ein USB-Massenspeicher eingesteckt ist, z.B. als /dev/sdc , so wird
# durch das Target 'usb-sdc' ein bootfähiges Live-System erstellt - ihr braucht
# dafür jedoch Rootzugriff.
# Alternativ könnt ihr die ISO-Datei einfach auf CD brennen.

iso: $(ISOFILE)

$(ISODIR)/$(ISOGRUBCFG):
	@echo "GEN		$@"
	@mkdir -p $(dir $@)
	@/bin/echo -e "set timeout=$(GRUBTIMEOUT)\nset default=0\n\nmenuentry \"$(shell id -un)s $(PROJECT)\" {\n\tmultiboot /$(ISOKERNEL)\n\tmodule /$(ISOINITRD)\n\tboot\n}" > $@

$(ISODIR)/$(ISOKERNEL): all
	@echo "CPY		$@"
	@mkdir -p $(dir $@)
	@cp -a $(KERNEL) $@

$(ISODIR)/$(ISOINITRD): all
	@echo "CPY		$@"
	@mkdir -p $(dir $@)
	@if [ -s $(INITRD) ] ; then cp -a $(INITRD) $@ ; else touch $@ ; fi

$(ISOFILE): $(ISODIR)/$(ISOKERNEL) $(ISODIR)/$(ISOINITRD) $(ISODIR)/$(ISOGRUBCFG)
	@echo "ISO		$@"
	@which $(XORRISO) >/dev/null || echo "Xorriso wurde nicht gefunden - sofern das erstellen der ISO fehl schlägt, kann dies eine der Ursachen sein!" >&2
	$(VERBOSE) $(MKISO) -d $(GRUBBIN) -o $@ $(ISODIR)

qemu-iso: $(ISOFILE)
	$(QEMU) -cdrom $< -smp $(QEMUCPUS) $(QEMUFLAGS)

kvm-iso: $(ISOFILE)
	$(QEMU) -cdrom $< -smp $(QEMUCPUS) $(KVMFLAGS)

usb: $(ISOFILE)
ifeq (,$(USBDEV))
	@echo "Benötigt Umgebungsvariable USBDEV mit Pfad zum USB-Speichergerät:" >&2
	@lsblk -o TYPE,KNAME,SIZE,MODEL -a -p | grep "^disk" | cut -b 6-
	@exit 1
else
	$(VERBOSE) $(DD) if=$< of=$(USBDEV) bs=4M status=progress && sync
endif

usb-%:
	@$(MAKE) USBDEV=/dev/$* usb

cd: $(ISOFILE)
ifeq ($(CDRWDEV),)
	@echo "Benötigt Umgebungsvariable CDRWDEV mit Pfad zum CD/DVD Brenner" >&2
	@exit 1
else
	$(VERBOSE) $(XORRISO) -as cdrecord -v dev=$(CDRWDEV) -dao $<
endif

cd-%:
	@$(MAKE) CDRWDEV=/dev/$* cd

# --------------------------------------------------------------------------
# Das Image wirds ins Verzeichnis des tftp-servers kopiert, um es ueber
# pxelinux auf den Testrechnern starten zu können.
netboot: all
	$(VERBOSE) initrd="$(INITRD)" ; \
	if [ ! -s "$$initrd" ] ; then \
		initrd="$(BUILDDIR)/fake-initrd" ; \
		echo "(none)" > "$$initrd" ; \
	fi ; \
	if [ -d  "$(NETBOOTDIR)" ] ; then \
		install -m 644 $(KERNEL) $(NETBOOTDIR)/$(shell id -run)/kernel ; \
		install -m 644 "$$initrd" $(NETBOOTDIR)/$(shell id -run)/initrd.img ; \
	else \
		tar --transform='flags=r;s|$(KERNEL)|kernel|' --transform='flags=r;s|$$initrd|initrd.img|' -cz $(KERNEL) "$$initrd" | \
		ssh "$(NETBOOTSSH)" "cat - | tar -tvz -C $(NETBOOTDIR)/\`id -run\`/" ; \
	fi

# --------------------------------------------------------------------------
# 'solution-1' startet die Musterloesung zur Aufgabe 1 mittels KVM.
# Andere Aufgabennummern können natürlich auch angegeben werden.
solution-%:
	$(QEMU) -kernel $(SOLUTIONDIR)/$(SOLUTIONPREFIX)$*.elf -initrd $(SOLUTIONDIR)/$(SOLUTIONPREFIX)$*.rd -smp $(QEMUCPUS) $(KVMFLAGS)

# --------------------------------------------------------------------------
# 'help' gibt - wenig überraschend - eine kurze Hilfe über die Targets aus
help:
	@/bin/echo -e "                              \e[1mM A K E F I L E\e[0m\n\n" \
		"\e[1mzum Uebungsbetriebssystem $(PROJECT)" \
		"der Lehrveranstaltung Betriebssysteme.\e[0m\n\n" \
		"Durch Eingabe von '\e[4mmake\e[0m' werden die Quelltexte des Systems kompiliert.\n" \
		"Es gibt Targets in jeweils zwei Varianten, \e[2;3m<name>\e[0m und \e[2;3m<name>\e[0;3m-noopt\e[0m.\n" \
		"Targets mit dem Suffix \e[3m-noopt\e[0m werden ohne Optimierungen kompiliert. Dies\n" \
		"kann sich z.B. für das Debugging mit \e[4mgdb\e[0m eignen.\n\n" \
		"Folgende Targets sind verfügbar (jeweils auch mit \e[3m-noopt\e[0m):\n" \
		"	\e[3mall\e[0m      Das System wird gebaut und liegt anschließend als ELF-Binary vor.\n\n" \
		"	\e[3mkvm\e[0m      KVM wird gestartet und führt euer System in einer\n" \
		"	         Hardware-Beschleunigten virtuellen Maschine aus.\n\n" \
		"	\e[3mnetboot\e[0m  Das System wird in das Netzwerkverzeichnis kopiert und kann auf den\n" \
		"	         Testrechner gestartet werden\n\n" \
		"	\e[3miso\e[0m      Erstellt ein bootfähiges Abbild für den Start auf Testrechner\n\n" \
		"	         mittels externe Datenträger (Datei: $(ISOFILE))\n\n" \
		"	\e[3mqemu\e[0m     QEMU wird gestartet und führt euer System aus. Da QEMU langsamer\n" \
		"	         ist als KVM treten hier u.U. andere Fehler hervor als in KVM oder\n" \
		"	         auf der Hardware.\n\n" \
		"	\e[3mqemu-gdb\e[0m QEMU wird mit aktiviertem GDB-Stub gestartet und eine GDB-Instanz\n" \
		"	         angehängt, damit lässt sich euer System Schritt für Schritt\n" \
		"	         debuggen\n\n" \
		"	\e[3mkvm-gdb\e[0m  Das selbe, aber mit Hardwarebeschleunigung\n\n" \
		"	\e[3mqemu-iso\e[0m Startet das System in QEMU über ein virtuelles CD Laufwerk\n\n" \
		"	\e[3mkvm-iso\e[0m  Das selbe, aber mit Hardwarebeschleunigung\n\n" \
		"\n	\e[3musb\e[0m      Erstellt ein bootfähiges System auf den mit der Umgebungsvariable\n" \
		"	         \e[4mUSBDEV\e[0m bestimmten USB Datenträger\n\n" \
		"\n	\e[3mcd\e[0m       Erstellt eine bootfähige CD auf den mit der Umgebungsvariable\n" \
		"	         \e[4mCDRWDEV\e[0m bestimmten CD-Brenner\n\n" \
		"\nAußerdem kann die Musterlösung im CIP mit dem Target\n" \
		"	\e[3msolution-\e[2;3mAufgabe\e[0m\n" \
		"(in KVM) ausgeführt werden, wobei für \e[2;3mAufgabe\e[0m die entsprechende Aufgabennummer\n" \
		"eingesetzt werden muss.\n\n" \
		""

# --------------------------------------------------------------------------
# Warnungen (falls vorhanden) anzeigen

ifeq (,$(COMPILER))
$(warning Unbekannter (und gegebenenfalls nicht unterstützter) Übersetzer "$(CXX)"!)
endif

# --------------------------------------------------------------------------
# Einbindung der Abhaengigkeitsdateien

ifneq ($(MAKECMDGOALS),clean)
-include $(DEP_FILES)
endif

.PHONY: all clean qemu qemu-gdb kvm-gdb kvm qemu-iso kvm-iso netboot help
