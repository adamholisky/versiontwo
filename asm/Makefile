SOURCES := $(wildcard *.asm)
OBJECTS := $(patsubst %.asm, ../build/%.o, $(SOURCES))

all: $(OBJECTS)

../build/%.o : %.asm
	$(ASM) $(AFLAGS) $< -o $@