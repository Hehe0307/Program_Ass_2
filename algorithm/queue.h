#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

// Interface class of 'queue'
class queueType {
    public: 
        queueType();
        void initializeQueue();
        bool isEmptyQueue();
        bool isFullQueue();
        void enqueue(int t);
        void dequeue();
        int queueFront;
    private:
        int maxQueueSize = 100, count, queueBack;
        int *q;
};

#endif