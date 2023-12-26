#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

class Queue {
    public: 
        void initialiseQueue();
        bool isEmptyQueue();
        bool isFullQueue();
        void enqueue(int t);
        void dequeue();
    private:
        int maxQueueSize = 100, count, queueFront, queueBack;
        int *q;
};

#endif