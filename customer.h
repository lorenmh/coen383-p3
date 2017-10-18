#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdlib.h>
#include <stdbool.h>
#include "seller.h"
#include <pthread.h>

typedef enum {
    HIGH_PRICE,
    MEDIUM_PRICE,
    LOW_PRICE
} price_t;

typedef struct {
    price_t priority;
} customer_t;

typedef struct {
    int N;
    seller_entry_t *entry;
    unsigned int seed;
    pthread_mutex_t *rand_mutex;
    pthread_barrier_t *start_signal;
    bool *need_stop;
} customer_thread_arg;


void customer_thread_arg_init(customer_thread_arg *arg, int N, seller_entry_t *entry, unsigned int seed,
                              pthread_mutex_t *rand_mutex, pthread_barrier_t *barrier, bool *all_seats_sold);

void* customer_creating_thread(void *arg);

#endif
