#include "wheel.h"
#include "leftWheel.h"
#include "Arduino.h"

// Implementation of 'leftWheel' class
leftWheel::leftWheel(const int left_pwm, const int left_dir1, const int left_dir2) : wheel::wheel(left_pwm, left_dir1, left_dir2) {}

void leftWheel::turnLeft() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir1, LOW);
  digitalWrite(left_dir2, HIGH);
}

void leftWheel::turnRight() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir1, HIGH);
  digitalWrite(left_dir2, LOW);
}
