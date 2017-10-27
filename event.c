#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "event.h"


struct node {
    event_t event;
    struct node *next;
};


struct event_pool {
    struct node *dummy_head;
    struct node *new_event_entry;
    pthread_mutex_t new_event_mutex;
};

void event_pool_destroy(event_pool_t *event_pool) {
    struct node *op = event_pool->dummy_head;
    while (op != NULL) {
        struct node *delete = op;
        op = op->next;
        free(delete);
    }
    pthread_mutex_destroy(&(event_pool->new_event_mutex));

}

event_pool_t *event_pool_init() {
    struct event_pool *new = malloc(sizeof(struct event_pool));
    new->dummy_head = malloc(sizeof(struct node));
    new->dummy_head->next = NULL;
    new->new_event_entry = new->dummy_head;
    pthread_mutex_init(&(new->new_event_mutex), NULL);
    return new;
}

void add_event(event_pool_t *event_pool, event_t event) {
    struct node *new_node = malloc(sizeof(struct node));
    new_node->next = NULL;
    new_node->event = event;
    pthread_mutex_lock(&(event_pool->new_event_mutex));
    event_pool->new_event_entry->next = new_node;
    event_pool->new_event_entry = new_node;
    pthread_mutex_unlock(&(event_pool->new_event_mutex));

}


