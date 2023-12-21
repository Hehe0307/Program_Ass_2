#include "Arduino.h"
#include "encoder.h"

Encoder::Encoder(int inter) : interruptPin(inter) {}

void Encoder::declarePin() {
  pinMode(interruptPin, INPUT);
}

void Encoder::calculate() {
  if (millis() - oldTime >= 1000) {
    noInterrupts(); 
    rpm = (60 * 1000 / diskPulse ) / (millis() - oldTime) * pulse; 
    velocity = rpm * 3.1416 * wheelDiameter * 60 / 1000000; 
    oldTime = millis();
    Serial.print(millis() / 1000); Serial.print("       ");
    Serial.print(rpm, DEC); Serial.print("   ");
    Serial.print(pulse, DEC); Serial.print("     ");
    Serial.println(velocity, 2); 
    pulse = 0;
    interrupts();
  }
}

volatile int Encoder::pulse = 0;
volatile unsigned int Encoder::debounce = 0;