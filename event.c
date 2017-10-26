#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "event.h"


struct node {
    event_t event;
    struct node *pre;
    struct node *next;
};


struct event_pool {
    pthread_t worker_ID;
    bool stop_worker;
    pthread_mutex_t last_mutex;
    struct node *last;
    pthread_cond_t worker_signal;
    pthread_mutex_t head_mutex;
    struct node *head;

};


void* worker(void *arg) {
    struct event_pool *pool = (struct event_pool*)arg;
    while (!(pool->stop_worker)) {
        pthread_mutex_lock(&(pool->head_mutex));
        if (pool->head != NULL) {
            pthread_cond_wait(&(pool->worker_signal), &(pool->head_mutex));
        }else {
            /// print that event
            struct node *temp = pool->head;
            pool->head = pool->head->next;
            free(temp);
        }
        pthread_mutex_unlock(&(pool->head_mutex));
    }
    return NULL;
}

void event_pool_destroy(event_pool_t *event_pool) {

}

event_pool_t *event_pool_init() {
    struct event_pool *new = malloc(sizeof(struct event_pool));
    new->stop_worker = false;
    new->last = NULL;
    new->head = NULL;
    pthread_mutex_init(&(new->last_mutex), NULL);
    pthread_mutex_init(&(new->head_mutex), NULL);
    pthread_cond_init(&(new->worker_signal), NULL);
    pthread_create(&(new->worker_ID), NULL, worker, new);
    return new;
}

void add_event(event_pool_t *event_pool, event_t event) {

    struct node *new_event_node = malloc(sizeof(struct node));
    new_event_node->event = event;
    new_event_node->next = NULL;
    new_event_node->pre = NULL;
    pthread_mutex_lock(&(event_pool->last_mutex));
    if (event_pool->last == NULL) {
        // head is NULL
        // worker is sleeping now
        event_pool->last = new_event_node;
        event_pool->head = new_event_node;
        pthread_cond_signal(&(event_pool->worker_signal));
    }else {
        event_pool->last->next = new_event_node;
        event_pool->last = new_event_node;
    }
    pthread_mutex_unlock(&(event_pool->last_mutex));
}


