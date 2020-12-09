#include "boot/multiboot/data.h"

struct multiboot_info {
	struct array {
		uint32_t size;
		uint32_t addr;
	} __attribute__((packed));

	enum Flag : uint32_t{
		Memory = 1 << 0,
		BootDev = 1 << 1,
		CmdLine = 1 << 2,
		Modules = 1 << 3,

		SymbolTable = 1 << 4,
		SectionHeader = 1 << 5,

		MemoryMap = 1 << 6,
		DriveInfo = 1 << 7,
		ConfigTable = 1 << 8,
		BootLoaderName = 1 << 9,
		ApmTable = 1 << 10,


		VbeInfo = 1 << 11,
		FramebufferInfo = 1 << 12
	} flags;


	struct {
		uint32_t lower;
		uint32_t upper;
	} mem  __attribute__((packed));
	uint32_t boot_device;
	uint32_t cmdline;
	struct array mods;
	union {

		struct {
			uint32_t tabsize;
			uint32_t strsize;
			uint32_t addr;
			uint32_t reserved;
		} aout_symbol_table  __attribute__((packed));


		struct {
			uint32_t num;
			uint32_t size;
			uint32_t addr;
			uint32_t shndx;
		} elf_section_header_table  __attribute__((packed));
	};

	struct array mmap;
	struct array drives;
	uint32_t config_table;
	uint32_t boot_loader_name;
	uint32_t apm_table;


	struct Multiboot::VBE vbe;
	struct Multiboot::Framebuffer framebuffer;


	bool hasFlag(enum Flag flag){
		return (flags & flag) != 0;
	}
} __attribute__((packed));
assert_size(multiboot_info, 116);


struct multiboot_info *multiboot_addr = 0;

namespace Multiboot {
	Module * getModule(unsigned i){
		if (multiboot_addr != nullptr && multiboot_addr->hasFlag(multiboot_info::Flag::Modules) && i < multiboot_addr->mods.size){
			return i + (Module*)((uintptr_t)multiboot_addr->mods.addr);
		} else
			return nullptr;
	}

	unsigned getModuleCount(){
		return multiboot_addr->mods.size;
	}

	void * Memory::getStartAddress(){
		if (sizeof(void*) == 4 && (addr >> 32) != 0)
			return (void*)0xffffffff;
		else
			return (void*)((uintptr_t)addr);
	}

	void * Memory::getEndAddress(){
		uint64_t end = addr + len;
		if (sizeof(void*) == 4 && (end >> 32) != 0)
			return (void*)0xffffffff;
		else
			return (void*)((uintptr_t)end);
	}

	bool Memory::isAvailable(){
		return type == Available;
	}

	Memory * Memory::getNext(){
		if (multiboot_addr != nullptr && multiboot_addr->hasFlag(multiboot_info::Flag::MemoryMap)){
			uintptr_t next = ((uintptr_t)this) + size + sizeof(size);
			if (next < multiboot_addr->mmap.addr + multiboot_addr->mmap.size)
				return (Memory *) next;
		}
		return nullptr;
	}

	Memory * getMemoryMap(){
		if (multiboot_addr != nullptr && multiboot_addr->hasFlag(multiboot_info::Flag::MemoryMap) && multiboot_addr->mmap.size > 0)
			return (Memory *)((uintptr_t)multiboot_addr->mmap.addr);
		else
			return nullptr;
	}

	char * getCommandLine(){
		return (char*)((uintptr_t)(multiboot_addr->cmdline));
	}

	char * getBootLoader(){
		return (char*)((uintptr_t)(multiboot_addr->boot_loader_name));
	}

	VBE * getVesaBiosExtensionInfo(){
		if (multiboot_addr != nullptr && multiboot_addr->hasFlag(multiboot_info::Flag::VbeInfo))
			return &(multiboot_addr->vbe);
		else
			return nullptr;
	}

	Framebuffer * getFramebufferInfo(){
		if (multiboot_addr != nullptr && multiboot_addr->hasFlag(multiboot_info::Flag::FramebufferInfo))
			return &(multiboot_addr->framebuffer);
		else
			return nullptr;
	}
}
