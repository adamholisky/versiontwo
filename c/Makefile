CC = /osdev/bin/i686-elf-gcc
CFLAGS = -ffreestanding -O2 -nostdlib -lgcc
ODIR = ../build

SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c, ../build/%.o, $(SOURCES))

all: $(OBJECTS)

../build/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@