ASM			= nasm
ASMFLAGS	= -f elf64 
LINKER 		= ld


all: build

build:
	 $(ASM) $(ASMFLAGS) forth.asm -o forth.o
	 $(LINKER) -o forth forth.o

clean:
	rm *.o
	rm forth