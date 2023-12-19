#include "wheel.h"
#include "Arduino.h"

// Implementation of 'wheel' class
wheel::wheel(const int wheel_pwm, const int wheel_dir1, const int wheel_dir2) : wheel_pwm(wheel_pwm), wheel_dir1(wheel_dir1), wheel_dir2(wheel_dir2) {
  pinMode(wheel_pwm, OUTPUT);
  pinMode(wheel_dir1, OUTPUT);
  pinMode(wheel_dir2, OUTPUT);
}

void wheel::moveForward() {
  digitalWrite(wheel_pwm, HIGH);
  digitalWrite(wheel_dir1, HIGH);
  digitalWrite(wheel_dir2, LOW);
}

void wheel::reverse() {
  digitalWrite(wheel_pwm, HIGH);
  digitalWrite(wheel_dir1, LOW);
  digitalWrite(wheel_dir2, HIGH);
}

void wheel::stop() {
  digitalWrite(wheel_pwm, LOW);
  digitalWrite(wheel_dir1, LOW);
  digitalWrite(wheel_dir2, LOW);
}
