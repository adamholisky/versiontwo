SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c, ../build/%.o, $(SOURCES))

all: $(OBJECTS)

../build/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@