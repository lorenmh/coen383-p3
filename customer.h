#ifndef CUSTOMER_H
#define CUSTOMER_H

typedef enum {
    HIGH_PRIORITY,
    MEDIUM_PRIORITY,
    LOW_PRIORITY
} priority_t;

typedef struct {
    priority_t priority;
    int seat_id;
    int seller_id;
    int arrival_time;
} customer_t;
