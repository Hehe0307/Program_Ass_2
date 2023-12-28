#include "ultrasonic.h"
#include "Arduino.h"

// Implementation of 'ultrasonic' class
ultrasonic::ultrasonic(const int trigPin, const int echoPin) : trigPin(trigPin), echoPin(echoPin) {}

void ultrasonic::declarePin() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void ultrasonic::retrieveData()
{
  long duration;
  digitalWrite(trigPin, LOW); // Reset the trigPin 
  delayMicroseconds(2); // delay for a small period of time 
  digitalWrite(trigPin, HIGH); // Set the trigPin to HIGH to emit ultrasonic waves
  delayMicroseconds(10); // delay used to allow trigPin to respond
  digitalWrite(trigPin, LOW); // Set the trigPin back to LOW
  duration = pulseIn(echoPin, HIGH); // retrieve raw data received at echoPin
  data = 0.0343 * duration / 2; // calculate distance using formula 0.0343 / 2 * duration 
}