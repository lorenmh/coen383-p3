#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "customer.h"
#include "seller.h"
#include "Seat.h"


int main(int argc, char *argv[]) {
    seller_entry_t price_H, price_L, price_M;
    seat_t seat_table[100];
    for (int i = 0; i < 100; ++i) {
        seat_table->available = false;
        pthread_spin_init(&(seat_table->lock), 0);
    }
    seller_entry_init(&price_H, 1, 'H', seat_table);
    seller_entry_init(&price_M, 3, 'M', seat_table);
    seller_entry_init(&price_L, 6, 'L', seat_table);

    pthread_barrier_t customer_start_signal;
    pthread_barrier_init(&customer_start_signal, NULL, 3);
    bool all_seats_sold = false;
    pthread_mutex_t rand_mutex;
    pthread_mutex_init(&rand_mutex, NULL);

    srand((unsigned int)time(NULL));
    unsigned int M_seed = (unsigned int)rand();
    unsigned int L_seed = (unsigned int)rand();
    unsigned int H_seed = (unsigned int)rand();

    customer_thread_arg arg_M;
    customer_thread_arg arg_H;
    customer_thread_arg arg_L;
    int N;

    customer_thread_arg_init(&arg_H, N, &price_H, H_seed, &rand_mutex, &customer_start_signal, &all_seats_sold);
    customer_thread_arg_init(&arg_M, N, &price_M, M_seed, &rand_mutex, &customer_start_signal, &all_seats_sold);
    customer_thread_arg_init(&arg_L, N, &price_L, L_seed, &rand_mutex, &customer_start_signal, &all_seats_sold);

    pthread_t customer_threads[3];
    pthread_create(&(customer_threads[0]), NULL, customer_creating_thread, &arg_M);
    pthread_create(&(customer_threads[0]), NULL, customer_creating_thread, &arg_H);
    pthread_create(&(customer_threads[0]), NULL, customer_creating_thread, &arg_L);





}
