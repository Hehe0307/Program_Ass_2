#include "wheel.h"
#include "leftWheel.h"
#include "Arduino.h"

// Implementation of 'leftWheel' class
leftWheel::leftWheel(const int left_pwm, const int left_dir_1, const int left_dir_2) : wheel(left_pwm, left_dir_1, left_dir_2) {}

void leftWheel::declarePin() {
  pinMode(left_pwm, OUTPUT);
  pinMode(left_dir_1, OUTPUT);
  pinMode(left_dir_2, OUTPUT);
}

void leftWheel::turnLeft() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir_1, LOW);
  digitalWrite(left_dir_2, HIGH);
}

void leftWheel::turnRight() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir_1, HIGH);
  digitalWrite(left_dir_2, LOW);
}
