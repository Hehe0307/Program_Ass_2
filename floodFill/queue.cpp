#include "queue.h"

queueType::queueType() {}

void queueType::initializeQueue(){
    queueFront = 0;
    queueBack = maxQueueSize - 1;
    count = 0;
}

bool queueType::isEmptyQueue(){
    return (count == 0);
}

bool queueType::isFullQueue(){
    return (count == maxQueueSize);
}

void queueType::enqueue(int t){
    queueBack = (queueBack + 1) % maxQueueSize;
    count++;
    q[queueBack] = t;
}

void queueType::dequeue(){
    count--;
    queueFront = (queueFront + 1) % maxQueueSize;
}
