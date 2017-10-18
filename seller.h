//
// Created by Yujian Zhang on 10/18/17.
//

#ifndef COEN383_P3_BUYINGWORKER_H
#define COEN383_P3_BUYINGWORKER_H


#include "Seat.h"
#include <pthread.h>
#include <stdlib.h>


typedef struct {
    int max_number_of_running_instance;
    seat_t *seat_table_entry;
    char ID;

    int number_of_running_instance;
    pthread_mutex_t new_thread_mutex;
    pthread_cond_t new_thread_CV;
    int quick_look_up_index;    // not accurate, but does not matter
    pthread_spinlock_t lookup_lock;
} seller_entry_t;

typedef struct {
    seller_entry_t *entry;
    char customerID[10];
} seller_thread_arg;

void seller_entry_init(seller_entry_t *entry, int max_running_instance, char ID, seat_t *seat_entry);
void* seller_selling_thread(void *arg);


#endif //COEN383_P3_BUYINGWORKER_H
