CC=gcc
CFLAGS=-std=gnu99
CMD=$(CC) $(CFLAGS)

COMPILE=$(CMD) -g -c -pthread -lm

FILES=main.o customer.o Seller.o Seat.o

all: main.o Seller.o customer.o Seat.o
	$(CMD) $(FILES) -pthread -lm -g -o app.bin

clean:
	rm *.o app.bin

main.o: main.c
	$(COMPILE) main.c

customer.o: customer.c customer.h
	$(COMPILE) customer.c

Seat.o: Seat.c Seat.h
	$(COMPILE) Seat.c

Seller.o: Seller.c Seller.h
	$(COMPILE) Seller.c