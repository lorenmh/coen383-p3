#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "customer.h"
#include "seller.h"
#include "seatFinder.h"
#include "event.h"

pthread_mutex_t lock;

void *seller(void *seller_args) {
    seller_args_t args = *((seller_args_t *) seller_args);

    customer_queue_t *current_queue = create_customer_queue(args.num_customers);
    customer_queue_t *completed_queue = create_completed_queue(args.num_customers);
    args.current_queue = current_queue;
    args.completed_queue = completed_queue;

	for(int i = 0; i < args.num_customers; i++){
		args.current_queue->buf[i].id = rand() % 100 + 1;
		sprintf(args.current_queue->buf[i].name, "%s:%d",args.name,args.current_queue->buf[i].id);
	}


    seatFinder(&args);

    return NULL;
}
