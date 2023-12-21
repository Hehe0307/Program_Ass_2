#include "Arduino.h"
#include "encoder.h"

Encoder::Encoder(int inter) : interruptPin(inter) {}

void Encoder::declarePin() {
  pinMode(interruptPin, INPUT);
}

void Encoder::calculate() {
  if (millis() - oldTime == 1000) // Update every second
  {  
      noInterrupts(); // Don't process interrupts during calculations // Turn off interrupt processing to perform calculations.
      rpm = (diskPulse/20)/60000; // Calculate revolutions per minute
      velocity = rpm * 3.1416 * 47 / 60000; // Calculate velocity in [m/s] 
      oldTime = millis(); // Store the current time.
      Serial.print(millis()/1000); Serial.print("       ");// Send time value, RPM, and pulses to the serial port.
      Serial.print(rpm,DEC); Serial.print("   ");
      Serial.print(diskPulse,DEC); Serial.print("     ");
      Serial.println(velocity,2); 
      diskPulse = 0;  // Initialize pulses.
      interrupts(); // Restart the interrupt processing // Restart interrupt processing
   }
}

volatile int Encoder::pulse = 0;
volatile unsigned int Encoder::debounce = 0;