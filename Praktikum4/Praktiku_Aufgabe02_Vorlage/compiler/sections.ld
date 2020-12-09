
ENTRY(startup_bsp)

SECTIONS
 {

	. = 16M;

	.boot :
	{

		*(.multiboot_header)
	}

	.text :
	{
		*(".text")
		*(".text$")
		*(".init")
		*(".fini")
		*(".gnu.linkonce.*")
	}


	.init_array :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
	}

	.data :
	{
		*(".data")
		*(".data$")
		*(".rodata")
		___CTOR_LIST__ = .;
		*(".ctors")
		*(".ctor")
		___CTOR_LIST_END__ = .;
		___DTOR_LIST__ = .;
		*(".dtors")
		*(".dtor")
		___DTOR_LIST_END__ = .;
		*(".got")
		*(".got.plt")
		*(".eh_frame")
		*(".eh_fram")
		*(".jcr")
	}


	.setup_ap_seg ALIGN(0x10) :
	{
		___SETUP_AP_START__ = .;
		*(".setup_ap_seg")
		*(".setup_ap_seg$")
	}
	___SETUP_AP_END__ = .;

	.bss :
	{
		*(".bss")
		*(".bss.*")
		*(COMMON)
	}
	___IMG_END___ = .;

	/DISCARD/ :
	{
		*(".note")
		*(".comment")

	}
 }
