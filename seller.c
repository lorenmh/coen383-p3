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

    customer_queue_t *current_queue = create_customer_queue(&args);
    args.current_queue = current_queue;

    seatFinder(&args);
    printf("%s has finished with %zu customers left \n", args.name, args.current_queue->size);

    return NULL;
}
