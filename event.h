#ifndef COEN383_P3_EVENT_H
#define COEN383_P3_EVENT_H

typedef enum {
    Arrived,
    Informed,
    left
} Event;

typedef struct {
    Event event;
    int seat_num;
    int time_stamp;
} event_t;


typedef struct event_pool event_pool_t;

event_pool_t* event_pool_init();
void event_pool_destroy(event_pool_t* event_pool);
void add_event(event_pool_t* event_pool, event_t event);
void print_events(event_pool_t* event_pool);
#endif //COEN383_P3_EVENT_H
