#ifndef CUSTOMER_H
#define CUSTOMER_H

#define MAX_ARRIVAL_TIME 59

typedef struct {
	time_t start;
	time_t end;
	time_t current;
} sys_time;

typedef enum {
    HIGH_PRIORITY,
    MEDIUM_PRIORITY,
    LOW_PRIORITY
} priority_t;
 
typedef struct {
    char name[10];
    int arrival_time;
    int id;
} customer_t;

typedef struct {
    size_t size;
    customer_t *buf;
} customer_queue_t;

extern struct event_pool *pool;

customer_queue_t *create_customer_queue(void *seller_args);

void print_customer_queue(customer_queue_t *queue);
long int current_time();

#endif
