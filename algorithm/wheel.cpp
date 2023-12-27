#include "wheel.h"
#include "Arduino.h"

// Implementation of 'wheel' class
wheel::wheel() {}

wheel::wheel(const int wheel_pwm, const int wheel_dir_1, const int wheel_dir_2) 
: wheel_pwm(wheel_pwm), wheel_dir_1(wheel_dir_1), wheel_dir_2(wheel_dir_2) {}

void wheel::declarePin() {
  pinMode(wheel_pwm, OUTPUT);
  pinMode(wheel_dir_1, OUTPUT);
  pinMode(wheel_dir_2, OUTPUT);
}

void wheel::moveForward() {
  analogWrite(wheel_pwm, 220);
  digitalWrite(wheel_dir_1, HIGH);
  digitalWrite(wheel_dir_2, LOW);
}

void wheel::moveStop() {
  analogWrite(wheel_pwm, 0);
  digitalWrite(wheel_dir_1, HIGH);
  digitalWrite(wheel_dir_2, LOW);
}