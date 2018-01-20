OBJECTS := $(wildcard ../build/*.o)

all:
	$(CC) -T linker.ld -o ../build/versiontwo.bin $(CFLAGS) ../build/boot.o $(OBJECTS)