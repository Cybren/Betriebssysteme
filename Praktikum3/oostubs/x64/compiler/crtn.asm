; C Runtime - Ende (muss entsprechend als letztes dem Linker übergeben werden)
[SECTION .init]
	; Der Compiler wird hier die Inhalte der .init Sektion von crtend.o einfügen
	pop rbp
	ret

[SECTION .fini]
	; Der Compiler wird hier die Inhalte der .fini Sektion von crtend.o einfügen
	pop rbp
	ret
