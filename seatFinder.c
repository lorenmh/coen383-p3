#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>

#include "customer.h"
#include "seller.h"
#include "seatFinder.h"

static int numTickets = SEAT_BUF_SIZE;

pthread_mutex_t lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

const int HSLEEP[2] = {1,2};
const int MSLEEP[3] = {2,3,4};
const int LSLEEP[4] = {4,5,6,7};

int random_int(int min, int max)
{
   return min + rand() % (max+1 - min);
}

void initLock(){
	//char *seat_buff = malloc(sizeof(char) * SEAT_BUF_SIZE);
	int rc = pthread_mutex_init(&lock, NULL);
	assert(rc == 0);	
}

void removeLock(){
	pthread_mutex_destroy(&lock);
}

void thread_sleep(void *seller_args){
	seller_args_t args = *((seller_args_t *) seller_args);
	int rand_t, s_time;
	
	if(args.priority == HIGH_PRIORITY){
		rand_t = rand() % 2;
		s_time = HSLEEP[rand_t];
		sleep(s_time);
	} else if(args.priority == MEDIUM_PRIORITY){
		rand_t = rand() % 3;
		s_time = MSLEEP[rand_t];
		sleep(s_time);
	} else{
		rand_t = rand() % 4;
		s_time = LSLEEP[rand_t];
		sleep(s_time);
	}
}




void *seatFinder(void *seller_args){
	seller_args_t args = *((seller_args_t *) seller_args);
	bool done = false;
	int current_arrival_time, tickets_sold, quanta = 0;
	args.current_index = 0;
	ending_quanta = 0;

	while(!done){
		pthread_mutex_lock(&lock);

		current_arrival_time = args.current_queue->buf[args.current_index].arrival_time;
		tickets_sold = args.current_queue->buf[args.current_index].tickets_wanted;

		//Still need to check if there are less seats than what the buyer wants
		if(numTickets < 0 || quanta > MAX_ARRIVAL_TIME){
			done = true;
		} else if (current_arrival_time == quanta){
			args.completed_queue->buf[args.current_index].arrival_time = current_arrival_time;
			args.completed_queue->buf[args.current_index].tickets_wanted = tickets_sold;

			numTickets -= tickets_sold;

			args.current_index++;
			args.current_queue->size--;

			printf("{%d} Seller {%s} has sold {%d} seats at arrival time {%d}. There are {%d} seats left.", quanta, args.name, tickets_sold, current_arrival_time, numTickets);
			printf("{%s} has {%zu} buyers left.\n", args.name, args.current_queue->size);
			
		
		}

		quanta++;

		pthread_mutex_unlock(&lock);

	}

	ending_quanta = quanta;
	return NULL;
	
}