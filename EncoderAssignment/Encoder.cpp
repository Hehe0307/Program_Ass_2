#include "Arduino.h"
#include "encoder.h"

Encoder::Encoder(int inter) : interruptPin(inter) {}

void Encoder::declarePin() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
}
