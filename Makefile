.DEFAULT_GOAL := runclean

CC = /osdev/bin/i686-elf-gcc
CFLAGS = -ffreestanding -O2 -nostdlib -lgcc -I../include -z muldefs

ASM = nasm
AFLAGS = -felf32

export

all: versiontwo.iso

versiontwo.iso:
	$(MAKE) -C asm
	$(MAKE) -C c
	$(MAKE) -C build-support
	cp build/versiontwo.bin -f iso/boot/versiontwo.bin
	grub-mkrescue -o versiontwo.iso iso

run: versiontwo.iso
	qemu-system-i386 -cdrom versiontwo.iso -serial stdio

runclean:
	make clean
	make run

clean:
	rm -f build/*.o
	rm -f build/*.bin
	rm -f versiontwo.iso