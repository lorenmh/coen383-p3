#ifndef SEATFINDER_H
#define SEATFINDER_H

#define SEAT_BUF_SIZE 100
#define ROW 10
#define COLUMN 10

extern const int HSLEEP[2];
extern const int MSLEEP[3];
extern const int LSLEEP[4];

const char* seat[][COLUMN];


void *seatFinder(void *seller_args);
void thread_sleep(void *seller_args);
void printSeats();

void initLock();

void initSeats();

void findSeat(void *seller_args);

void initSleepTime();

void removeLock();

#endif