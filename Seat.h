//
// Created by Yujian Zhang on 10/18/17.
//

#ifndef COEN383_P3_SEAT_H
#define COEN383_P3_SEAT_H

#include <stdbool.h>
#include <pthread.h>





typedef struct {
    bool available;
    pthread_spinlock_t lock;
} seat_t;


void seat_init(seat_t *seat);



#endif //COEN383_P3_SEAT_H
