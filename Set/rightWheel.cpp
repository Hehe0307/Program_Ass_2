#include "wheel.h"
#include "rightWheel.h"
#include "Arduino.h"

// Implementation of 'rightWheel' class
rightWheel::rightWheel(const int right_pwm, const int right_dir1, const int right_dir2) : wheel::wheel(right_pwm, right_dir1, right_dir2) {}

void rightWheel::turnLeft() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir1, HIGH);
  digitalWrite(right_dir2, LOW);
}

void rightWheel::turnRight() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir1, LOW);
  digitalWrite(right_dir2, HIGH);
}