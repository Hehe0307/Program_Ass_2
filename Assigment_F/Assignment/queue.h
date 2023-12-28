#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

// Interface class of 'queueType'
class queueType {
    public: 
        queueType();
        void initializeQueue(); // To initialize the queue
        bool isEmptyQueue(); // To check if the queue is empty
        bool isFullQueue(); // To check if the queue is full
        void enqueue(int t); // To add a data value into the queue
        int dequeue(); // To remove a data value into the queue
        int getFront();
        int getBack();
        int *q;

    private:
        int maxQueueSize = 144, count, queueBack, queueFront; 
        
};

#endif