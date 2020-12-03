/* Einsprungspunkt in unser System - Label startup_bsp in der boot/startup.asm */
ENTRY(startup_bsp)

SECTIONS
 {
	/* Startadresse des Systems */
	. = 16M;

	.boot :
	{
		/* Der Multiboot Header sollte ganz am Anfang sein */
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

	/* Liste von Startadressen der globalen Konstruktoren (benoetigt ab gcc 4.7) */
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

	/* Startcode fuer die APs, wird von System::init() reloziert */
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
/*
		*(".debug_line")
		*(".debug_info")
		*(".debug_abbrev")
		*(".debug_aranges")
*/
	}
 }
