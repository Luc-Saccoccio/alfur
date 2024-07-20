all: 42 hello_asm hello_c

42:
	tcc 42.c -c -o 42.o
	tcc 42.o -o 42

hello_c:
	tcc hello.c -c -o hello_c.o
	tcc hello_c.o -o hello_c

hello_asm:
	nasm -w+all -f elf64 -o hello_asm.o hello.asm
	ld hello_asm.o -o hello_asm

clean:
	rm -f 42 hello_c hello_asm *.o
