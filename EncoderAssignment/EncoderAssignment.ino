#include "Encoder.h"
#include "constant.h"
#include "Arduino.h"

Encoder leftEncoder(LEFT_ENC);
// Encoder rightEncoder(RIGHT_ENC);

void counter()
{
    if(digitalRead (encoderPin) && (micros()-debounce > 500) && digitalRead (encoderPin) ) 
    { 
// Check again if the encoder sends a valid signal and then check if the time is greater than 1000 microseconds, and verify again that the signal is correct.
    debounce = micros(); // Store the time to check for bounce in the signal.
    pulse++;
    }  // Add a valid pulse.
}

void setup(){
  Serial.begin(115200);
  leftEncoder.declarePin();
  //ightEncoder.declarePin();
  Serial.print("Seconds ");
  Serial.print("RPM ");
  Serial.print("Pulses ");
  Serial.println("Velocity[Km/h]");
  attachInterrupt(digitalPinToInterrupt(2), counter, RISING);
}

void loop()
{
  Serial.println("Start");
  leftEncoder.calculate();
  counter();
  Serial.println("Start");

}