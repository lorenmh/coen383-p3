#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "customer.h"
#include "seller.h"

void *customer_creating_thread(void *arg) {
    customer_thread_arg *customer_arg = (customer_thread_arg*)arg;

    unsigned int arrival_interval[2 * (customer_arg->N)];
    pthread_t worker_ID[2 * (customer_arg->N)];
    memset(arrival_interval, 0, sizeof(int) * 2 * (customer_arg->N));
    memset(worker_ID, 0, sizeof(pthread_t) * 2 * (customer_arg->N));
    pthread_mutex_lock(customer_arg->rand_mutex);
    srand(customer_arg->seed);
    for (int i = 0; i < 2 * customer_arg->N; ++i) {
        // exponential distribution
        double random = ((double)rand()) / ((double)(RAND_MAX));
        arrival_interval[i] = (unsigned int)(log(1 - random) / ((double)(-customer_arg->N)));
    }
    pthread_mutex_unlock(customer_arg->rand_mutex);

    int current_process = 0;

    pthread_barrier_wait(customer_arg->start_signal);


    while (!*(customer_arg->need_stop) && current_process < 2 * customer_arg->N) {
        sleep(arrival_interval[current_process]);
        seller_thread_arg *new_arg = malloc(sizeof(seller_thread_arg));
        new_arg->entry = customer_arg->entry;
        sprintf(new_arg->customerID, "%c%d", (customer_arg->entry->ID), current_process);
        pthread_create(&worker_ID[current_process], NULL, seller_selling_thread, (void *) new_arg);
        current_process += 1;
    }



    return NULL;
}

void customer_thread_arg_init(customer_thread_arg *arg, int N, seller_entry_t *entry, unsigned int seed,
                              pthread_mutex_t *rand_mutex, pthread_barrier_t *barrier, bool *all_seats_sold) {
    arg->entry = entry;
    arg->need_stop = all_seats_sold;
    arg->start_signal = barrier;
    arg->rand_mutex = rand_mutex;
    arg->seed = seed;
    arg->N = N;
}
