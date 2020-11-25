; C Runtime - Anfang (muss entsprechend als erstes dem Linker übergeben werden)
[SECTION .init]
[GLOBAL _init]
_init:
	push rbp
	mov rbp, rsp
	; Der Compiler wird hier die Inhalte der .init Sektion von crtbegin.o einfügen

[SECTION .fini]
[GLOBAL _fini]
_fini:
	push rbp
	mov rbp, rsp
	; Der Compiler wird hier die Inhalte der .fini Sektion von crtbegin.o einfügen
