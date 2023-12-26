#include "encoder.h"
#include "Arduino.h"

encoder::encoder(const int _encoderPin, const int _interruptPin) 
: encoderPin(_encoderPin), interruptPin(_interruptPin) {}

void encoder::declarePin() {
    pinMode(encoderPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(encoderPin), counter, RISING);
}

void encoder::counter() { pulse++; }

void encoder::getPulse() { cli(); sei(); }

void encoder::enable() { getPulseTask.enable(); }

void encoder::check() { getPulseTask.check(); }