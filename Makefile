CC = gcc
CCFLAGS = -std=gnu89 -O2

OBJS = main.o xmalloc.o fatal.o cplus-dem.o

.PHONY: all clean

all: cplus-dem

cplus-dem: $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) -c $(CCFLAGS) $< -o $@
	
clean:
	rm $(OBJS) cplus-dem
