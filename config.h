#ifndef CONFIG_H 
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"

#define MAX_QUEUE 100

typedef struct {
    Process* data[MAX_QUEUE];
    int front;
    int rear;
} Queue;

void init_queue(Queue* q) {
    q->front = 0;
    q->rear = 0;
}

bool is_empty(Queue* q) {
    return q->front == q->rear;
}

bool is_full(Queue* q) {
    return q->rear == MAX_QUEUE;
}

void enqueue(Queue* q, Process* p) {
    if (is_full(q)) {
        fprintf(stderr, "Queue overflow\n");
        return;
    }
    q->data[q->rear++] = p;  // rear¿¡ »ðÀÔ    
}

Process* dequeue(Queue* q) {
    if (is_empty(q)) return NULL;
    return q->data[q->front++];  // front¿¡¼­ ²¨³»±â
}


//Queue ready_q;
//Queue wait_q; 

#endif