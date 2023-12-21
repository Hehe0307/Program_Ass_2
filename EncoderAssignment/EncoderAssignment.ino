#include "Arduino.h"
#include "constant.h"
#include "encoder.h"

Encoder leftEncoder(LEFT_PIN);
// Encoder rightEncoder(RIGHT_PIN);

void leftCounter() {
  if(digitalRead(LEFT_PIN) && (micros() - leftEncoder.debounce > 500) && digitalRead(LEFT_PIN)) { 
  leftEncoder.debounce = micros();
  leftEncoder.pulse++;
  }
}

// void rightCounter() {
//   if(digitalRead(RIGHT_PIN) && (micros() - rightEncoder.debounce > 500) && digitalRead(RIGHT_PIN)) { 
//   rightEncoder.debounce = micros();
//   rightEncoder.pulse++;
//   }
// }

void setup() {
  Serial.begin(9600);
  leftEncoder.declarePin();
  attachInterrupt(0, leftCounter, RISING);
  // attachInterrupt(0, rightCounter, RISING);
}

void loop() {
  leftEncoder.calculate();
  leftCounter();
  // rightEncoder.calculate();
  // rightCounter();
}