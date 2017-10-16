#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    int num_customers = atoi(argv[1]);

    printf("Num Customers: %d\n", num_customers);
}
