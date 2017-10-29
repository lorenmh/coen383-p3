CC=gcc
CFLAGS=-std=c99
CMD=$(CC) $(CFLAGS)

COMPILE=$(CMD) -g -c -pthread

#FILES=Main.o MinHeap.o Process.o RandomNumberGenerator.o Output.o Scheduler.o
FILES=main.o customer.o seller.o event.o

all: main.o customer.o seller.o event.o
	$(CMD) $(FILES) -g -o app.bin

clean:
	rm *.o app.bin

main.o: main.c
	$(COMPILE) main.c

customer.o: customer.c customer.h
	$(COMPILE) customer.c

seller.o: seller.c seller.h
	$(COMPILE) seller.c

event.o: event.c event.h
	$(COMPILE) event.c