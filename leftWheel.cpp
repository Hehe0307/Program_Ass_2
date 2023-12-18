#include "wheel.h"
#include "leftWheel.h"
#include "Arduino.h"

// Implementation of 'leftWheel' class
leftWheel::leftWheel(const int left_pwm, const int left_dir) : wheel::wheel(left_pwm, left_dir) {}

void leftWheel::turnLeft() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir, LOW);
}

void leftWheel::turnRight() {
  digitalWrite(left_pwm, LOW);
  digitalWrite(left_dir, HIGH);
}
