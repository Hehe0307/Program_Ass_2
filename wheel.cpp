#include "wheel.h"
#include "Arduino.h"

// Implementation of 'wheel' class
wheel::wheel(const int wheel_pwm, const int wheel_dir) : wheel_pwm(wheel_pwm), wheel_dir(wheel_dir) {
  pinMode(wheel_pwm, OUTPUT);
  pinMode(wheel_dir, OUTPUT);
}

void wheel::moveForward() {
  digitalWrite(wheel_pwm, HIGH);
  digitalWrite(wheel_dir, LOW);
}

void wheel::reverse() {
  digitalWrite(wheel_pwm, LOW);
  digitalWrite(wheel_dir, HIGH);
}

void wheel::stop() {
  digitalWrite(wheel_pwm, LOW);
  digitalWrite(wheel_dir, LOW);
}
