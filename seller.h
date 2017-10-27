#ifndef SELLER_H
#define SELLER_H

int ending_quanta;

typedef struct {
    customer_queue_t *current_queue;
    customer_queue_t *completed_queue;
    priority_t priority;
    char name[3];
    int id;
    int num_customers;
    int current_index;
} seller_args_t;

void *seller(void *args);

#endif
