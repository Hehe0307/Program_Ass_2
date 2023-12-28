#include "wheel.h"
#include "Arduino.h"

// Implementation of 'wheel' class
wheel::wheel() {}

wheel::wheel(const int wheel_pwm, const int wheel_dir_1, const int wheel_dir_2, int speed) 
: wheel_pwm(wheel_pwm), wheel_dir_1(wheel_dir_1), wheel_dir_2(wheel_dir_2), speed(speed) {}

void wheel::declarePin() {
  pinMode(wheel_pwm, OUTPUT);
  pinMode(wheel_dir_1, OUTPUT);
  pinMode(wheel_dir_2, OUTPUT);
}

void wheel::moveForward() {
  analogWrite(wheel_pwm, speed);
  digitalWrite(wheel_dir_1, HIGH);
  digitalWrite(wheel_dir_2, LOW);
}

void wheel::moveReverse() {
  analogWrite(wheel_pwm, speed);
  digitalWrite(wheel_dir_1, LOW);
  digitalWrite(wheel_dir_2, HIGH);
}

void wheel::moveStop() {
  analogWrite(wheel_pwm, 0);
  digitalWrite(wheel_dir_1, HIGH);
  digitalWrite(wheel_dir_2, LOW);
}