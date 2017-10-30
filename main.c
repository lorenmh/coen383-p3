#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "customer.h"
#include "seller.h"
#include "seatFinder.h"
#include "event.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int num_customers;

    printf("\nEnter the value of customers per queue: ");
    scanf("%d", &num_customers);

    seller_args_t args[10];
    pthread_t threads[10];
    
    struct event_pool *pool = event_pool_init();

    initSeats();    
    initLock();

    for (int i = 0; i < 10; i++) {
        if (i == 0) {
            args[i] = (seller_args_t) { NULL, NULL,HIGH_PRIORITY, {0}, i, num_customers };
            sprintf(args[i].name, "H");
        } else if (i > 0 && i < 4) {
            args[i] = (seller_args_t) { NULL, NULL,MEDIUM_PRIORITY, {0}, i, num_customers };
            sprintf(args[i].name, "M%d", i);
        } else {
            args[i] = (seller_args_t) { NULL, NULL,LOW_PRIORITY, {0}, i, num_customers };
            sprintf(args[i].name, "L%d", i - 3);
        }

        if (pthread_create(&threads[i], NULL, seller, &args[i])) {
            printf("error creating thread\n");
        }
    }

    for (int i = 0; i < 10; i++) {
        if (pthread_join(threads[i], NULL)) {
            printf("error joining thread\n");
        }
    }

    event_pool_destroy(pool);
    removeLock();


}
