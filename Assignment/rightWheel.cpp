#include "wheel.h"
#include "rightWheel.h"
#include "Arduino.h"

// Implementation of 'rightWheel' class
rightWheel::rightWheel(const int right_pwm, const int right_dir_1, const int right_dir_2) : wheel(right_pwm, right_dir_1, right_dir_2) {}

void rightWheel::declarePin() {
  pinMode(right_pwm, OUTPUT);
  pinMode(right_dir_1, OUTPUT);
  pinMode(right_dir_2, OUTPUT);
}

void rightWheel::turnLeft() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir_1, HIGH);
  digitalWrite(right_dir_2, LOW);
}

void rightWheel::turnRight() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir_1, LOW);
  digitalWrite(right_dir_2, HIGH);
}