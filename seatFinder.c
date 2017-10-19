#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include "customer.h"
#include "seller.h"

void *seatFinder(void *seller_args){
	seller_args_t args = *((seller_args_t *) seller_args);

	printf(" Seller {%s\n} has found a seat", args.name);

	return NULL;
}