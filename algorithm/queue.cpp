#include "queue.h"

void Queue::initialiseQueue(){
    queueFront = 0;
    queueBack = maxQueueSize - 1;
    count = 0;
}

bool Queue::isEmptyQueue(){
    return (count == 0);
}

bool Queue::isFullQueue(){
    return (count == maxQueueSize);
}

void Queue::enqueue(int t){
    queueBack = (queueBack + 1) % maxQueueSize;
    count++;
    q[queueBack] = t;
}

void Queue::dequeue(){
    count--;
    queueFront = (queueFront + 1) % maxQueueSize;
}
