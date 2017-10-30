#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>



#include "customer.h"
#include "seller.h"
#include "seatFinder.h"
#include "event.h"

static int numTickets = SEAT_BUF_SIZE;
struct event_pool *pool;

pthread_mutex_t lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int middle_flag = 0;
char* seating [ROW][COLUMN];

const int HSLEEP[2] = {1,2};
const int MSLEEP[3] = {2,3,4};
const int LSLEEP[4] = {4,5,6,7};

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
	event_t leave_event;
	
	leave_event.event = Left;
	sprintf(leave_event.seller_id, "%s", args.name);
    sprintf(leave_event.customer_id, "%s.%d", args.name, args.current_index);
	leave_event.time_stamp = current_time();
	add_event(pool, leave_event);

	if(args.priority == HIGH_PRIORITY){
		rand_t = rand() % 2;
		s_time = HSLEEP[rand_t];
		sleep(s_time);
		//usleep(s_time * 1000);
	} else if(args.priority == MEDIUM_PRIORITY){
		rand_t = rand() % 3;
		s_time = MSLEEP[rand_t];
		sleep(s_time);
		//usleep(s_time * 1000);
	} else if(args.priority == LOW_PRIORITY){
		rand_t = rand() % 4;
		s_time = LSLEEP[rand_t];
		sleep(s_time);
		//usleep(s_time * 1000);
	}
}

void initSeats(){
	for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            seating[row][col] = malloc(2);
            strcpy(seating[row][col], "X");
        }
    }  
}


void printSeats(){
	
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	for (int row = 0; row < ROW; row++)
	{
		for (int col = 0; col < COLUMN; col++)
		{
			printf("%8.8s", seating[row][col]);
		}
		printf("\n");
	}	
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void findSeat(void *seller_args, int customer_index){
	seller_args_t args = *((seller_args_t *) seller_args);

	event_t seat_event;
	
	seat_event.event = Informed;
	sprintf(seat_event.seller_id, "%s", args.name);
    sprintf(seat_event.customer_id, "%s.%d", args.name, args.current_index);
	seat_event.time_stamp = current_time();


	if(args.priority == HIGH_PRIORITY){
		
		for(int i = 0; i < ROW; i++){
			for(int j = 0; j < COLUMN; j++){
				if(strcmp(seating[i][j], "X") == 0){
					seating[i][j] = args.current_queue->buf[customer_index].name;
					seat_event.seat_num = (i*10)+j+1;
					add_event(pool, seat_event);
					return;
				}
			}
		}
	} else if(args.priority == MEDIUM_PRIORITY){
		if(middle_flag == 0){
			for(int i = 5; i < ROW; i++){
				for(int j = 0; j < COLUMN; j++){
				  if(strcmp(seating[i][j], "X") == 0){
						seating[i][j] = args.current_queue->buf[customer_index].name;
						seat_event.seat_num = (i*10)+j+1;
						add_event(pool, seat_event);
						middle_flag = 1;
						return;
					}
				}
			}
		} else if(middle_flag == 1){
			for(int i = 5; i >= 0;i--){
				for(int j = 0; j < COLUMN; j++){
			       if(strcmp(seating[i][j], "X") == 0){
						seating[i][j] = args.current_queue->buf[customer_index].name;
						seat_event.seat_num = (i*10)+j+1;
						add_event(pool, seat_event);
						middle_flag = 0;
						return;
					}
				}
			}
		}
	} else if(args.priority == LOW_PRIORITY){
		for(int i = ROW - 1; i >= 0; i--){
			for(int j = 0; j < COLUMN; j++){
				if(strcmp(seating[i][j], "X") == 0){
					seating[i][j] = args.current_queue->buf[customer_index].name;
					seat_event.seat_num = (i*10)+j+1;
					add_event(pool, seat_event);
					return;
				}
			}
		}
	}


}


void *seatFinder(void *seller_args){
	seller_args_t args = *((seller_args_t *) seller_args);
	bool done = false;
	int quanta = 0;
	ending_quanta = 0;
	start_time();

	while(!done){
		pthread_mutex_lock(&lock);

		end_time();
		int temp_time = current_time();

		if(numTickets == 0 || current_time() > MAX_ARRIVAL_TIME || args.current_queue->size <= 0){
				done = true;
		} else{
			event_t arrival_event;
            arrival_event.time_stamp = current_time();
            arrival_event.event = Arrived;
            sprintf(arrival_event.seller_id, "%s", args.name);
            sprintf(arrival_event.customer_id, "%s.%d", args.name, args.current_index);
            add_event(pool, arrival_event);
			
			findSeat(&args, args.current_index);

			numTickets--;

			args.current_queue->size--;
			args.current_index++;
		}
		
		quanta++;
		pthread_mutex_unlock(&lock);
		thread_sleep(&args);
	}

	ending_quanta = quanta;
	return NULL;
}
