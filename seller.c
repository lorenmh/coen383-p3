#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "customer.h"
#include "seller.h"
#include "seatFinder.h"

pthread_mutex_t lock;

void *seller(void *seller_args) {
    seller_args_t args = *((seller_args_t *) seller_args);

    //printf("{%s} id: %d, priority: %d\n", args.name, args.id, args.priority);

    customer_queue_t *current_queue = create_customer_queue(10);
    customer_queue_t *completed_queue = create_completed_queue(10);
    args.current_queue = current_queue;
    args.completed_queue = completed_queue;

	for(int i = 0; i < args.current_queue->size; i++){
		sprintf(args.current_queue->buf[i].name, "%s:%d",args.name,i);
	}


	//pthread_mutex_lock(&lock);
    //printf("Seller {%s} Queue:  ", args.name);
    //print_customer_queue(args.current_queue);
    //printf("%s\n", args.name);
	//pthread_mutex_unlock(&lock);

    seatFinder(&args);

    printf("{%s} has sold {%lu} with {%zu} customers remaining\n", args.name, 10 - args.current_queue->size, args.current_queue->size);
    return NULL;
}
