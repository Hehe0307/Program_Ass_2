#include "wheel.h"
#include "Arduino.h"

// Implementation of 'wheel' class
wheel::wheel(const int wheel_pwm, const int wheel_dir_1, const int wheel_dir_2) : wheel_pwm(wheel_pwm), wheel_dir_1(wheel_dir_1), wheel_dir_2(wheel_dir_2) {}

void wheel::moveForward() {
  digitalWrite(wheel_pwm, HIGH);
  digitalWrite(wheel_dir_1, HIGH);
  digitalWrite(wheel_dir_2, LOW);
}

void wheel::reverse() {
  digitalWrite(wheel_pwm, HIGH);
  digitalWrite(wheel_dir_1, LOW);
  digitalWrite(wheel_dir_2, HIGH);
}

void wheel::stop() {
  digitalWrite(wheel_pwm, LOW);
  digitalWrite(wheel_dir_1, HIGH);
  digitalWrite(wheel_dir_2, LOW);
}
