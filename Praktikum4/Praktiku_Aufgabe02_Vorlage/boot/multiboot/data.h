#pragma once

#include "types.h"
#include "compiler/fix.h"

namespace Multiboot {
	class Module {
		uint32_t start;
		uint32_t end;
		uint32_t cmdline;
		uint32_t pad UNUSED_STRUCT_FIELD;
	public:
		void * getStartAddress(){
			return (void*)((uintptr_t)start);
		}
		void * getEndAddress(){
			return (void*)((uintptr_t)end);
		}
		size_t getSize(){
			return (size_t)(end-start);
		}
		char * getCommandLine(){
			return (char*)((uintptr_t)cmdline);
		}
	} __attribute__((packed));
	assert_size(Module, 16);

	Module * getModule(unsigned);
	unsigned getModuleCount();

	char * getCommandLine();
	char * getBootLoader();

	class Memory {
		uint32_t size;
		uint64_t addr;
		uint64_t len;
		enum Type : uint32_t {
			Available = 1,
			Reserved = 2
		} type;
	public:
		void * getStartAddress();
		void * getEndAddress();
		bool isAvailable();
		Memory * getNext();
	} __attribute__((packed));
	assert_size(Memory, 24);

	Memory * getMemoryMap();


	struct VBE {
		uint32_t control_info;
		uint32_t mode_info;
		uint16_t mode;
		uint16_t interface_seg;
		uint16_t interface_off;
		uint16_t interface_len;
	} __attribute__((packed));
	assert_size(VBE, 16);

	VBE * getVesaBiosExtensionInfo();


	struct Framebuffer {
		uint64_t address;
		uint32_t pitch;
		uint32_t width;
		uint32_t height;
		uint8_t bpp;
		enum Type : uint8_t {
			INDEXED = 0,
			RGB = 1,
			EGA_TEXT = 2
		} type;
		union {
			struct {
				uint32_t palette_addr;
				uint16_t palette_num_colors;
			} __attribute__((packed));
			struct {
				uint8_t offset_red;
				uint8_t bits_red;
				uint8_t offset_green;
				uint8_t bits_green;
				uint8_t offset_blue;
				uint8_t bits_blue;
			} __attribute__((packed));
		} __attribute__((packed));
	} __attribute__((packed));
	assert_size(Framebuffer, 28);

	Framebuffer * getFramebufferInfo();
}
