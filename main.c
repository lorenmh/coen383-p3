#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void *hello(void *id) {
    sleep(rand() % 4);

    printf("Hello from %d\n", (int) id);

    return id;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    //if (argc != 2) return 1;
    //int num_customers = atoi(argv[1]);

    //printf("Num Customers: %d\n", num_customers);

    pthread_t threads[10];

    for (long i = 0; i < 10; i++) {
        if (pthread_create(&threads[i], NULL, hello, (void *) i)) {
            printf("error creating thread\n");
        }
    }

    for (int i = 0; i < 10; i++) {
        if (pthread_join(threads[i], NULL)) {
            printf("error joining thread\n");
        }
    }
}
