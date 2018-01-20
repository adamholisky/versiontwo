SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c, ../build/%.o, $(SOURCES))

all: $(OBJECTS) ../build/setjmp.o

../build/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

../build/setjmp.o : setjmp.S 
	$(CC) $(CFLAGS) -c $< -o $@