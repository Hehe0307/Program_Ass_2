#ifndef ENCODER_H
#define ENCODER_H

class Encoder {
  public:
    Encoder(int);
    void declarePin();
    void calculate();
    double value = 0;
    int interruptPin;
  private:
    const int wheelDiameter = 47;
    const int pulsePerGrid = 24;
};

#endif