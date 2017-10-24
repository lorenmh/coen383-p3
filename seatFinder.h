#ifndef SEATFINDER_H
#define SEATFINDER_H

#define SEAT_BUF_SIZE 100

extern const int HSLEEP[2];
extern const int MSLEEP[3];
extern const int LSLEEP[4];

void *seatFinder(void *seller_args);
void thread_sleep(void *seller_args);

void initLock();

void initSleepTime();

void removeLock();

#endif