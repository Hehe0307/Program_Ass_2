#include "wheel.h"
#include "rightWheel.h"
#include "Arduino.h"

// Implementation of 'rightWheel' class
rightWheel::rightWheel(const int right_pwm, const int right_dir) : wheel::wheel(right_pwm, right_dir) {}

void rightWheel::turnLeft() {
  digitalWrite(right_pwm, LOW);
  digitalWrite(right_dir, HIGH);
}

void rightWheel::turnRight() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir, LOW);
}