#include "Arduino.h"
#include "constant.h"
#include "encoder.h"

Encoder leftEncoder(LEFT_PIN);

void counter() {
  if(digitalRead(LEFT_PIN) && (micros() - leftEncoder.debounce > 500) && digitalRead(LEFT_PIN)) { 
  leftEncoder.debounce = micros();
  leftEncoder.pulse++;
  }
}

void setup() {
  Serial.begin(9600);
  leftEncoder.declarePin();
  attachInterrupt(0, counter, RISING);
}

void loop() {
  leftEncoder.calculate();
  counter();
}