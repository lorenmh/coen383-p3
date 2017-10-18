//
// Created by Yujian Zhang on 10/18/17.
//

#include "seller.h"


void seller_entry_init(seller_entry_t *entry, int max_running_instance, char ID, seat_t *seat_entry) {
    entry->number_of_running_instance = 0;
    entry->max_number_of_running_instance = max_running_instance;
    pthread_cond_init(&(entry->new_thread_CV), 0);
    pthread_mutex_init(&(entry->new_thread_mutex), 0);
    pthread_spin_init(&(entry->lookup_lock), 0);
    entry->ID = ID;
    entry->seat_table_entry = seat_entry;
    switch (ID) {
        case 'H':
            entry->quick_look_up_index = 0;
        case 'M':
            entry->quick_look_up_index = 50;
        case 'L':
            entry->quick_look_up_index = 99;
    }

}

void *seller_selling_thread(void *arg) {

    seller_thread_arg *entry = (seller_thread_arg*)arg;







    free(entry);
    return NULL;
}


