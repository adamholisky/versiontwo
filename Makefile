.DEFAULT_GOAL := runclean

CC = /osdev/bin/i686-elf-gcc
CFLAGS = -ffreestanding -O2 -nostdlib -static-libgcc -lgcc -isystem../libc-includes -I../include -I../duktape -z muldefs

ASM = nasm
AFLAGS = -felf32

export

all: versiontwo.iso

versiontwo.iso:
	$(MAKE) -C asm
	$(MAKE) -C c
	$(MAKE) -C libc
	#$(MAKE) -C duktape
	$(MAKE) -C commands
	$(MAKE) -C build-support
	cp build/versiontwo.bin -f iso/boot/versiontwo.bin
	grub-mkrescue -d /usr/lib/grub/i386-pc -o versiontwo.iso iso

run: versiontwo.iso
	qemu-system-i386 -device isa-debug-exit,iobase=0xf4,iosize=0x04 -cdrom versiontwo.iso -serial stdio -serial file:serial_out.txt -m 4G

runclean:
	make clean
	make run

clean:
	rm -f build/*.o
	rm -f build/*.bin
	rm -f versiontwo.iso
