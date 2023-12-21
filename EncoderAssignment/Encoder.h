#ifndef ENCODER_H
#define ENCODER_H

class Encoder {
  public:
    Encoder(int);
    void declarePin();
    void calculate();
    static volatile int pulse;
    static volatile unsigned int debounce;
  private:
    int interruptPin;
    const int wheelDiameter = 47;
    const int diskPulse = 20;
    unsigned int oldTime = 0;
    unsigned int rpm = 0;
    float velocity = 0;
    int input;
};

#endif