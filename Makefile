CC=gcc
CFLAGS=-std=c99
CMD=$(CC) $(CFLAGS)

COMPILE=$(CMD) -g -c

#FILES=Main.o MinHeap.o Process.o RandomNumberGenerator.o Output.o Scheduler.o
FILES=main.o

all: main.o
	$(CMD) $(FILES) -g -o app.bin

clean:
	rm *.o app.bin

main.o: main.c
	$(COMPILE) main.c
