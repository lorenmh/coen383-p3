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
#include "event.h"

static int numTickets = SEAT_BUF_SIZE;

pthread_mutex_t lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int middle_flag = 0;
const char* seating [ROW][COLUMN];


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
		sleep(s_time);
	} else if(args.priority == MEDIUM_PRIORITY){
		rand_t = rand() % 3;
		s_time = MSLEEP[rand_t];
		sleep(s_time);
	} else if(args.priority == LOW_PRIORITY){
		rand_t = rand() % 4;
		s_time = LSLEEP[rand_t];
		sleep(s_time);
	}
}

void initSeats(){
	for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            seating[row][col] = "X";
            printf(" { %s } ", seating[row][col]);
        }
        printf("\n");
    }  
}


void printSeats(){
	
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	for (int row = 0; row < ROW; row++)
	{
		for (int col = 0; col < COLUMN; col++)
		{
			printf(" { %s } ", seating[row][col]);
		}
		printf("\n");
	}	
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

void findSeat(void *seller_args, int customer_index){
	seller_args_t args = *((seller_args_t *) seller_args);
	int tickets_sold = args.current_queue->buf[args.current_index].tickets_wanted;
	int seatAssign;

	if(args.priority == HIGH_PRIORITY){
		
		for(int i = 0; i < ROW; i++){
			for(int j = 0; j < COLUMN; j++){
				if(seating[i][j] == "X"){
					seatAssign = (i*10)+j+1;
					seating[i][j] = args.current_queue->buf[customer_index].name;
					return;
				}
			}
		}
	} else if(args.priority == MEDIUM_PRIORITY){
		if(middle_flag == 0){
			for(int i = 5; i < ROW; i++){
				for(int j = 0; j < COLUMN; j++){
					if(seating[i][j] == "X"){
						seatAssign = (i*10)+j+1;
						seating[i][j] = args.current_queue->buf[customer_index].name;
						return;
					}
				}
				middle_flag = 1;
			}
		} else if(middle_flag == 1){
			for(int i = 5; i >= 0;i--){
				for(int j = 0; j < COLUMN; j++){
					if(seating[i][j] == "X"){
						seatAssign = (i*10)+j+1;
						seating[i][j] = args.current_queue->buf[customer_index].name;
						return;
					}
				}
				middle_flag = 0;
			}
		}
	} else if(args.priority == LOW_PRIORITY){
		for(int i = ROW - 1; i >= 0; i--){
			for(int j = 0; j < COLUMN; j++){
				if(seating[i][j] == "X"){
					seatAssign = (i*10)+j+1;
					seating[i][j] = args.current_queue->buf[customer_index].name;
					return;
				}
			}
		}
	}


}


void *seatFinder(void *seller_args){
	seller_args_t args = *((seller_args_t *) seller_args);
	bool done = false;
	int current_arrival_time, tickets_sold,  quanta = 0;
	args.current_index = 0;
	ending_quanta = 0;

	while(!done){
		pthread_mutex_lock(&lock);

		current_arrival_time = args.current_queue->buf[args.current_index].arrival_time;
		tickets_sold = args.current_queue->buf[args.current_index].tickets_wanted;

		if(numTickets <= 0 || quanta > MAX_ARRIVAL_TIME){
			done = true;
		}
		if((numTickets - tickets_sold) < 0){
			quanta++;
			pthread_mutex_unlock(&lock);
			continue;
		} else if (current_arrival_time == quanta){
			
			args.completed_queue->buf[args.current_index].arrival_time = current_arrival_time;
			args.completed_queue->buf[args.current_index].tickets_wanted = tickets_sold;
			
			int temp = tickets_sold;
			while (tickets_sold != 0){
				findSeat(&args, args.current_index);
				tickets_sold--;
			} 
			printf("%s sold %d tickets. There are %d tickets remaining\n", args.name, temp, numTickets);
			
			numTickets -= temp;

			args.current_index++;
			args.current_queue->size--;

			printSeats();
			thread_sleep(&args);
			
			args.current_index++;
			args.current_queue->size--;
			numTickets-=tickets_sold;
		}
		quanta++;
		pthread_mutex_unlock(&lock);
	}

	ending_quanta = quanta;
	return NULL;
}