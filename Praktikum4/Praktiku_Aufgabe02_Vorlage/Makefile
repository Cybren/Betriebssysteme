.DEFAULT_GOAL = all

INITRD_DIR ?= initrd/
INITRD_TOOL ?= fs/tool/fstool
INITRD_DEP =
INITRD_FREE ?= 1048576

LINKER_SCRIPT = compiler/sections.ld
CRTI_SOURCE = compiler/crti.asm
CRTN_SOURCE = compiler/crtn.asm
CC_SOURCES = $(shell find * -name "*.cc" -a ! -name '.*' -a ! -path 'test*' -a ! -path 'fs/tool/*' -a ! -path 'graphics/*')
ASM_SOURCES = $(shell find * -name "*.asm"  -a ! -name '.*')

KERNEL = $(BUILDDIR)/system
KERNEL64 = $(KERNEL)64
ISOFILE = $(BUILDDIR)/stubs.iso

include common.mk

ifneq ($(wildcard $(INITRD_DIR)*),)
INITRD = $(BUILDDIR)/initrd.img
INITRD_DEP += $(shell find $(INITRD_DIR) -type f )
$(KERNEL): $(INITRD)
endif

all: $(KERNEL)

$(KERNEL64): $(CRTI_OBJECT) $(CRTN_OBJECT) $(ASM_OBJECTS) $(CC_OBJECTS) $(LINKER_SCRIPT) $(MAKEFILE_LIST)
	@echo "LD		$@"
	@mkdir -p $(@D)
	$(VERBOSE) $(CXX) $(CXXFLAGS) -Wl,-T $(LINKER_SCRIPT) -o $@ $(LDFLAGS) $(CRTI_OBJECT) $(CRTBEGIN_OBJECT) $(ASM_OBJECTS) $(CC_OBJECTS) $(CRTEND_OBJECT) $(CRTN_OBJECT) $(LIBGCC)

$(KERNEL): $(KERNEL64)
	$(VERBOSE) objcopy -I elf64-x86-64 -O elf32-i386 $< $@

$(INITRD_TOOL): $(shell test -d $(dir $(INITRD_TOOL)) && find $(dir $(INITRD_TOOL)) -name "*.cc" -or -name '*.h')
	@echo "Make		$@"
	@make -C $(dir $(INITRD_TOOL))

$(INITRD): $(INITRD_TOOL) $(INITRD_DEP)
	@echo "INITRD		$@"
	@dd if=/dev/zero of=$@ bs=$(shell du -s $(INITRD_DIR) | cut -f1 | xargs expr $(INITRD_FREE) + ) count=1
	@/sbin/mkfs.minix -3 $@  # optional --inodes <number>
	@./$(INITRD_TOOL) put "$(INITRD_DIR)" $@
