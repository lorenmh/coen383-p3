#include <stdio.h>
#include <stdlib.h>

#include "customer.h"

const int tickets[3] = {1,2,3};

int customer_comparator(const void *a, const void *b) {
    customer_t *cust_a = (customer_t *) a;
    customer_t *cust_b = (customer_t *) b;

    return cust_a->arrival_time - cust_b->arrival_time;
}

customer_queue_t *create_customer_queue(int num_customers) {
    int rand_t, t_wanted;
    size_t customer_t_sz = sizeof(customer_t);

    customer_queue_t *queue = malloc(sizeof(customer_queue_t));

    queue->size = num_customers;
    queue->buf = calloc(num_customers, customer_t_sz);

    for (int i = 0; i < num_customers; i++) {
        queue->buf[i].arrival_time = rand() % MAX_ARRIVAL_TIME;

        rand_t = rand() % 3;
        t_wanted = tickets[rand_t];
        queue->buf[i].tickets_wanted = t_wanted;
    }

    qsort(queue->buf, num_customers, customer_t_sz, customer_comparator);

    return queue;
}

customer_queue_t *create_completed_queue(int num_customers){
    size_t customer_t_sz = sizeof(customer_t);
    customer_queue_t *queue = malloc(sizeof(customer_queue_t));
    
    queue->size = 0;
    queue->buf = calloc(num_customers, customer_t_sz);

    return queue;
}


void print_customer_queue(customer_queue_t *queue) {
    printf("[ ");
    for (int i = 0; i < queue->size; i++) {
        printf("{at: %d with %d tickets} ", queue->buf[i].arrival_time,queue->buf[i].tickets_wanted);
    }
    printf("]\n");
}
