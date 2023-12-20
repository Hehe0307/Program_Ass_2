#include "Encoder.h"
#include "Arduino.h"

// Encoder::Encoder(int encoderPin, int input) : encoderPin(encoderPin), input(input) {}

Encoder::Encoder(int encoderPin) : encoderPin(encoderPin){}

void Encoder::declarePin()
{
  pinMode(encoderPin, INPUT_PULLUP);
}

void Encoder::calculate()
{
  if (millis()- oldTime >= 1000) //Update the calculations every second
  {
    noInterrupts(); // Don't process interrupts during calculations // Turn off interrupt processing to perform calculations.
    rpm = (60 * 1000 / diskPulse )/ (millis() - oldTime)* pulse; // Calculate revolutions per minute
    velocity = rpm * 3.1416 * wheelDiameter * 60 / 1000000; // Calculate velocity in [Km/h] 
    oldTime = millis(); // Store the current time.
    Serial.print(millis()/1000); Serial.print("       ");// Send time value, RPM, and pulses to the serial port.
    Serial.print(rpm,DEC); Serial.print("   ");
    Serial.print(pulse,DEC); Serial.print("     ");
    Serial.println(velocity,2); 
    pulse = 0;  // Initialize pulses.
    interrupts(); // Rest
  }
}


volatile unsigned int Encoder::debounce = 0;

