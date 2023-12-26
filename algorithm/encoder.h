#ifndef ENCODER_H_INCLUDED
#define ENCODER_H_INCLUDED

#include <TimedAction.h>
#include "constant.h"

class encoder {
    public:
        encoder(const int, const int);
        void declarePin();
        void counter();
        void getPulse();
        void enable();
        void check();
        TimedAction getPulseTask = TimedAction(TASK_INTERVAL, getPulse);
        volatile uint32_t pulse = 0;
    private:
        const int encoderPin;
        const int interruptPin;
};

#endif // ENCODER_H_INCLUDED