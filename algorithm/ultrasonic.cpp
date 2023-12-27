#include "ultrasonic.h"
#include "Arduino.h"

// Implementation of 'ultrasonic' class
ultrasonic::ultrasonic(const int trigPin, const int echoPin) : trigPin(trigPin), echoPin(echoPin) {}

void ultrasonic::declarePin() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long ultrasonic::retrieveData()
{
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // delays are required for a succesful sensor operation.
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // this delay is required as well!
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  long distance = 0.0343 * duration / 2;
  return distance; 
}