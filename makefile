# Descobre automaticamente todos os arquivos .c e .h na pasta kernel/
C_SOURCES = $(wildcard kernel/*.c)
HEADERS = $(wildcard kernel/*.h)

# Converte os arquivos .c em arquivos de objeto .o
OBJ = ${C_SOURCES:.c=.o}

# Ferramentas do sistema (compilador cruzado e montador)
CC = gcc
LD = ld
ASM = nasm

# Flags essenciais para compilação Bare-Metal (32-bits, sem biblioteca padrão)
CFLAGS = -m32 -fno-pie -ffreestanding -fno-builtin -nostdlib -Wall -Wextra

# Alvo principal: gera a imagem de disco final do sistema operacional
all: os-image.bin

# Roda o kernel emulado no QEMU (ótimo para testar direto no terminal web)
run: os-image.bin
	qemu-system-i386 -drive format=raw,file=os-image.bin

# Concatena o bootloader com o binário do kernel para formar a imagem de boot
os-image.bin: boot/bootloader.bin kernel.bin
	cat $^ > os-image.bin

# Compila o setor de boot (Bootloader)
boot/bootloader.bin: boot/bootloader.asm
	${ASM} -f bin $< -o $@

# Liga todos os objetos usando o script linker.ld e gera o kernel binário puro
kernel.bin: kernel/kernel_entry.o ${OBJ}
	${LD} -m elf_i386 -o $@ -T linker.ld $^ --oformat binary

# Regra genérica para compilar arquivos .c em .o
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

# Regra genérica para compilar arquivos Assembly (.asm) em arquivos .o de 32-bits
%.o: %.asm
	${ASM} -f elf32 $< -o $@

# Limpa os arquivos gerados na compilação anterior
clean:
	rm -rf *.bin *.o kernel/*.o boot/*.bin os-image.bin
