#ifndef ENCODER_H
#define ENCODER_H

#include "Arduino.h"

class Encoder
{
    public:
        Encoder(int);
        void declarePin();
        void calculate();

    protected:
        const int wheelDiameter = 47;
        const int diskPulse = 20;
        unsigned int oldTime = 0;
        unsigned int rpm = 0;
        volatile int pulse = 0;
        float velocity = 0;
        static volatile unsigned int debounce;
        int encoderPin;
        int input;

};

#endif