[SECTION .multiboot_header]

%include 'boot/multiboot/config.inc'

align 4
multiboot_header:
	dd MULTIBOOT_HEADER_MAGIC_OS
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_HEADER_CHKSUM
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	dd MULTIBOOT_VIDEO_WIDTH
	dd MULTIBOOT_VIDEO_HEIGHT
	dd MULTIBOOT_VIDEO_BITDEPTH
