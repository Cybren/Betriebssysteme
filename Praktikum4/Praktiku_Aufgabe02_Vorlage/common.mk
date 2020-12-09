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

VPATH = $(sort $(dir $(CC_SOURCES) $(ASM_SOURCES)))

CRTI_OBJECT = $(addprefix $(BUILDDIR)/,$(addsuffix .o,$(CRTI_SOURCE)))
CRTN_OBJECT = $(addprefix $(BUILDDIR)/,$(addsuffix .o,$(CRTN_SOURCE)))
CC_OBJECTS = $(addprefix $(BUILDDIR)/,$(CC_SOURCES:.cc=.o))
DEP_FILES = $(addprefix $(BUILDDIR)/,$(CC_SOURCES:.cc=.d) $(addsuffix .d,$(ASM_SOURCES)))
ASM_OBJECTS = $(addprefix $(BUILDDIR)/,$(addsuffix .o,$(filter-out $(CRTI_SOURCE) $(CRTN_SOURCE),$(ASM_SOURCES))))

$(BUILDDIR)/%.d : %.cc $(MAKEFILE_LIST)
	@echo "DEP		$<"
	@mkdir -p $(@D)
	$(VERBOSE) $(CXX) $(CXXFLAGS) -MM -MT $(BUILDDIR)/$*.o -MF $@ $<

$(BUILDDIR)/%.asm.d : %.asm $(MAKEFILE_LIST)
	@echo "DEP		$<"
	@mkdir -p $(@D)
	$(VERBOSE) $(ASM) $(ASMFLAGS) -MT $(BUILDDIR)/$*.asm.o -MF $@ $<

$(BUILDDIR)/%.o : %.cc $(MAKEFILE_LIST)
	@echo "CXX		$<"
	@mkdir -p $(@D)
	$(VERBOSE) $(CXX) -c $(CXXFLAGS) -o $@ $<

$(BUILDDIR)/%.asm.o : %.asm $(MAKEFILE_LIST)
	@echo "ASM		$<"
	@mkdir -p $(@D)
	$(VERBOSE) $(ASM) $(ASMFLAGS) -o $@ $<

clean:
	@echo "RM		$(BUILDDIR)"
	$(VERBOSE) rm -rf "$(BUILDDIR)"
	$(VERBOSE) rm -rf "$(BUILDDIR)$(NOOPTTAG)"
	$(VERBOSE) rm -rf "$(BUILDDIR)$(VERBOSETAG)"
	@echo "RM		$(ISODIR)"
	$(VERBOSE) rm -rf "$(ISODIR)"
	$(VERBOSE) rm -rf "$(ISODIR)$(NOOPTTAG)"
	$(VERBOSE) rm -rf "$(ISODIR)$(VERBOSETAG)"

%-noopt:
	make BUILDDIR="$(BUILDDIR)$(NOOPTTAG)" ISODIR="$(ISODIR)$(NOOPTTAG)" OPTFLAGS="-O0" $*

%-verbose:
	make BUILDDIR="$(BUILDDIR)$(VERBOSETAG)" ISODIR="$(ISODIR)$(VERBOSETAG)" OPTFLAGS="-DVERBOSE" $*

qemu: all
	$(QEMU) -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) $(QEMUFLAGS)

qemu-gdb: all
	gdb $(KERNEL) \
		-ex "set arch i386:x86-64" \
		-ex "target remote | exec $(QEMU) -gdb stdio -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) -S $(QEMUFLAGS) $(QEMUDBGFLAGS)"

kvm-gdb: all
	gdb $(KERNEL) \
		-ex "set arch i386:x86-64" \
		-ex "target remote | exec $(QEMU) -gdb stdio -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) -S $(KVMFLAGS) $(QEMUDBGFLAGS)"

kvm: all
	$(QEMU) -kernel $(KERNEL) -initrd $(INITRD) -smp $(QEMUCPUS) $(KVMFLAGS)

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

solution-%:
	$(QEMU) -kernel $(SOLUTIONDIR)/$(SOLUTIONPREFIX)$*.elf -initrd $(SOLUTIONDIR)/$(SOLUTIONPREFIX)$*.rd -smp $(QEMUCPUS) $(KVMFLAGS)

ifeq (,$(COMPILER))
$(warning Unbekannter (und gegebenenfalls nicht unterstützter) Übersetzer "$(CXX)"!)
endif

ifneq ($(MAKECMDGOALS),clean)
-include $(DEP_FILES)
endif

.PHONY: all clean qemu qemu-gdb kvm-gdb kvm qemu-iso kvm-iso netboot help
