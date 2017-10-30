#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#include "customer.h"
#include "event.h"
#include "seller.h"


struct event_pool *pool;
sys_time t_time;

int customer_comparator(const void *a, const void *b) {
    customer_t *cust_a = (customer_t *) a;
    customer_t *cust_b = (customer_t *) b;

    return cust_a->arrival_time - cust_b->arrival_time;
}

customer_queue_t *create_customer_queue(void *seller_args) {
    int rand_t, t_wanted;
    size_t customer_t_sz = sizeof(customer_t);
    seller_args_t args = *((seller_args_t *) seller_args);


    customer_queue_t *queue = malloc(sizeof(customer_queue_t));

    queue->size = args.num_customers;
    queue->buf = calloc(args.num_customers, customer_t_sz);

    for (int i = 0; i < args.num_customers; i++) {
        queue->buf[i].arrival_time = rand() % MAX_ARRIVAL_TIME;
        
        queue->buf[i].id = i;
        sprintf(queue->buf[i].name, "%s:%d",args.name,i);

        event_t event;
        event.time_stamp = queue->buf[i].arrival_time;
        //add_event(pool, event); 
    }

    qsort(queue->buf, args.num_customers, customer_t_sz, customer_comparator);

    return queue;
}


void print_customer_queue(customer_queue_t *queue) {
    printf("[ ");
    for (int i = 0; i < queue->size; i++) {
        printf("{%s at: %d} ", queue->buf[i].name,queue->buf[i].arrival_time);
    }
    printf("]\n");
}

long int current_time(){
     t_time.current = t_time.end - t_time.start;
     return t_time.current;
}

void start_time(){
    t_time.start = time(NULL);
}


void end_time(){
    t_time.end = time(NULL);
}