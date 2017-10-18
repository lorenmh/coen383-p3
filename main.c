#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "customer.h"
#include "seller.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    //if (argc != 2) return 1;
    //int num_customers = atoi(argv[1]);

    //printf("Num Customers: %d\n", num_customers);

    seller_args_t args[10];
    pthread_t threads[10];

    for (int i = 0; i < 10; i++) {
        if (i == 0) {
            args[i] = (seller_args_t) { NULL, HIGH_PRIORITY, {0}, i };
            sprintf(args[i].name, "H");
        } else if (i > 0 && i < 4) {
            args[i] = (seller_args_t) { NULL, MEDIUM_PRIORITY, {0}, i };
            sprintf(args[i].name, "M%d", i);
        } else {
            args[i] = (seller_args_t) { NULL, LOW_PRIORITY, {0}, i };
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
}
