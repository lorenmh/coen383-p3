#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "customer.h"
#include "seller.h"
#include "seatFinder.h"

static int numTickets = SEAT_BUF_SIZE;

pthread_mutex_t lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int middle_flag = 0;

const int HSLEEP[2] = {1,2};
const int MSLEEP[3] = {2,3,4};
const int LSLEEP[4] = {4,5,6,7};

int random_int(int min, int max)
{
   return min + rand() % (max+1 - min);
}

void initLock(){
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
		//printf(" High %d\n", s_time);
		sleep(1);
	} else if(args.priority == MEDIUM_PRIORITY){
		rand_t = rand() % 3;
		s_time = MSLEEP[rand_t];
		//printf("Mid %d\n", s_time);
		sleep(1);
	} else if(args.priority == LOW_PRIORITY){
		rand_t = rand() % 4;
		s_time = LSLEEP[rand_t];
		//printf("Low %d\n", s_time);
		sleep(1);
	}
}

void printSeats(int seats[][COLUMN], int rowMax, int colMax){
	
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	for (int row = 0; row < rowMax; row++)
	{
		for (int col = 0; col < colMax; col++)
		{
			printf(" { %d } ", seats[row][col]);
		}
		printf("\n");
	}	
}

void findSeat(void *seller_args , int seats[][COLUMN]){
	seller_args_t args = *((seller_args_t *) seller_args);
	int tickets_sold = args.current_queue->buf[args.current_index].tickets_wanted;

	if(args.priority == HIGH_PRIORITY){
		
		for(int i = 0; i < ROW; i++){
			for(int j = 0; j < COLUMN; j++){
				if(seats[i][j] == 0){
					int seatAssign = (i*10)+j+1;
					seats[i][j] = seatAssign;
					tickets_sold--;
					return;
				}
			}
		}
		
		
		
	} else if(args.priority == MEDIUM_PRIORITY){
		if(middle_flag == 0){
			for(int i = 5; i < ROW; i++){
				for(int j = 0; j < COLUMN; j++){
					if(seats[i][j] == 0){
						int seatAssign = (i*10)+j+1;
						seats[i][j] = seatAssign;
						return;
					}
				}
				middle_flag = 1;
			}
		} else if(middle_flag == 1){
			for(int i = 5; i >= 0;i--){
				for(int j = 0; j < COLUMN; j++){
					if(seats[i][j] == 0){
						int seatAssign = (i*10)+j+1;
						seats[i][j] = seatAssign;
						return;
					}
				}
				middle_flag = 0;
			}
		}
		
		

	} else if(args.priority == LOW_PRIORITY){
		for(int i = ROW - 1; i >= 0; i--){
			for(int j = 0; j < COLUMN; j++){
				if(seats[i][j] == 0){
					int seatAssign = (i*10)+j+1;
					seats[i][j] = seatAssign;
					return;
				}
			}
		}
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
			
			int temp = tickets_sold;
			while (tickets_sold != 0){
				findSeat(&args,seats);
				tickets_sold--;
			} 
			printf("%s sold %d tickets. There are %d tickets remaining\n", args.name, temp, numTickets);
			

			numTickets -= temp;

			args.current_index++;
			args.current_queue->size--;

			//printf("{%d} Seller {%s} has sold {%d} seats at arrival time {%d}. There are {%d} seats left.", quanta, args.name, tickets_sold, current_arrival_time, numTickets);
			//printf("{%s} has {%zu} buyers left.\n", args.name, args.current_queue->size);
			printSeats(seats, ROW, COLUMN);
			//
			//printf("%s ", args.name);

			thread_sleep(&args);

		}


		quanta++;
		pthread_mutex_unlock(&lock);
	}

	ending_quanta = quanta;
	return NULL;
}