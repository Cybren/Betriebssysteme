# -----------------------------------------------------------------------------
#
#                              M A K E F I L E
#
# Zum Uebungsbetriebssystem OO/MP-StuBS der Lehrveranstaltung Betriebssysteme.
# -----------------------------------------------------------------------------
#
# Durch Eingabe von 'make' werden die Quelltexte des Systems kompiliert.
# Es gibt Targets in mehreren Varianten, <name>, <name>-noopt sowie
# <name>-verbose.
# Targets mit dem Suffix -noopt werden ohne Optimierungen kompiliert. Dies
# kann sich z.B. für das Debugging mit gdb eignen.
# Bei dem Suffix -verbose werden umfangreiche Ausgaben (über den Ausgabestrom
# DBG_VERBOSE) angezeigt, was beim printf-Debugging hilfreich sein kann.
#
# Folgende Targets sind verfügbar (jeweils auch mit -noopt):
# all:      Das System wird gebaut und liegt anschließend als ELF-Binary vor.
#
# kvm:      KVM wird gestartet und führt euer System in einer
#           Hardware-Beschleunigten virtuellen Maschine aus. Dieses Target ruft
#           aus Sicherhietsgründen ein CIP-Pool spezifisches Script auf.
#
# netboot:  Das System wird in das Netzwerkverzeichnis kopiert und kann auf den
#           Testrechner gestartet werden
#
# iso:      Erstellt ein bootfähiges Abbild für den Start auf Testrechner
#           mittels externe Datenträger
#
# qemu:     QEMU wird gestartet und führt euer System aus. Da QEMU langsamer
#           ist als KVM treten hier u.U. andere Fehler hervor als in KVM oder
#           auf der Hardware.
#
# qemu-iso: QEMU start und führt euer System von der ISO (als CD eingelegt) aus
#
# qemu-gdb: QEMU wird mit aktiviertem GDB-Stub gestartet und eine GDB-Instanz
#           angehängt, damit lässt sich euer System Schritt für Schritt
#           debuggen
#
# qemu-ddd: Wie qemu-gdb nur dient hier DDD als Frontend.
#
# help:     Zeigt eine umfangreiche Hilfe an
#
# -----------------------------------------------------------------------------

# --------------------------------------------------------------------------
# Default target

.DEFAULT_GOAL = all

# --------------------------------------------------------------------------
# Pfad zum Ordner für Dateien für die initiale Ramdisk (für Aufgabe 7)
INITRD_DIR ?= initrd/
INITRD_TOOL ?= fs/tool/fstool
INITRD_DEP =
# 1MB Freier Speicher
INITRD_FREE ?= 1048576

# --------------------------------------------------------------------------
# Quelldateien
# Linker Script
LINKER_SCRIPT = compiler/sections.ld
# Zu uebersetzende Quelldateien (Wichtig: vor common.mk definieren!)
CRTI_SOURCE = compiler/crti.asm
CRTN_SOURCE = compiler/crtn.asm
CC_SOURCES = $(shell find * -name "*.cc" -a ! -name '.*' -a ! -path 'test*' -a ! -path 'fs/tool/*' -a ! -path 'graphics/*')
ASM_SOURCES = $(shell find * -name "*.asm"  -a ! -name '.*')

# -----------------------------------------------------------------------------
# Parameter:
KERNEL = $(BUILDDIR)/system
KERNEL64 = $(KERNEL)64
ISOFILE = $(BUILDDIR)/stubs.iso

# --------------------------------------------------------------------------
# Globale Variablen und Standard-Rezepte einbinden
include common.mk

# -----------------------------------------------------------------------------
# Initiale Ramdisk
ifneq ($(wildcard $(INITRD_DIR)*),)
INITRD = $(BUILDDIR)/initrd.img
INITRD_DEP += $(shell find $(INITRD_DIR) -type f )
# zusätzliche Abhängigkeit für Kernel definieren
$(KERNEL): $(INITRD)
endif

LIBS = liboostubs.a

all: $(KERNEL) $(LIBS)

# Rezept zum Linken des Systemimages
# Wir nutzen dafür den C++ Compiler(dieser ruft dann den tatsächlichen Linker auf)
$(KERNEL64): $(CRTI_OBJECT) $(CRTN_OBJECT) $(ASM_OBJECTS) $(CC_OBJECTS) $(LINKER_SCRIPT) $(MAKEFILE_LIST)
	@echo "LD		$@"
	@mkdir -p $(@D)
	$(VERBOSE) $(CXX) $(CXXFLAGS) -Wl,-T $(LINKER_SCRIPT) -o $@ $(LDFLAGS) $(CRTI_OBJECT) $(CRTBEGIN_OBJECT) $(ASM_OBJECTS) $(CC_OBJECTS) $(CRTEND_OBJECT) $(CRTN_OBJECT) $(LIBGCC) $(LIBS)

# Der Kernel muss für Multiboot ein 32bit elf sein
$(KERNEL): $(KERNEL64) $(LIBS)
	$(VERBOSE) objcopy -I elf64-x86-64 -O elf32-i386 $< $@

# Tool zum Bearbeiten eines Minix-v3 Abbildes (Aufgabe 7)
$(INITRD_TOOL): $(shell test -d $(dir $(INITRD_TOOL)) && find $(dir $(INITRD_TOOL)) -name "*.cc" -or -name '*.h')
	@echo "Make		$@"
	@make -C $(dir $(INITRD_TOOL))

# Ramdisk mit Minix v3 Dateisystem
$(INITRD): $(INITRD_TOOL) $(INITRD_DEP)
	@echo "INITRD		$@"
	@dd if=/dev/zero of=$@ bs=$(shell du -s $(INITRD_DIR) | cut -f1 | xargs expr $(INITRD_FREE) + ) count=1
	@/sbin/mkfs.minix -3 $@  # optional --inodes <number>
	@./$(INITRD_TOOL) put "$(INITRD_DIR)" $@

