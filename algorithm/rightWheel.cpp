#include "wheel.h"
#include "rightWheel.h"
#include "Arduino.h"

// Implementation of 'rightWheel' class
rightWheel::rightWheel(const int right_pwm, const int right_dir_1, const int right_dir_2) 
: right_pwm(right_pwm), right_dir_1(right_dir_1), right_dir_2(right_dir_2) {}

void rightWheel::declarePin() {
  wheel::wheel(right_pwm, right_dir_1, right_dir_2);
}

void rightWheel::moveForward() {
  analogWrite(right_pwm, 220);
  digitalWrite(right_dir_1, HIGH);
  digitalWrite(right_dir_2, LOW);
}

void rightWheel::moveStop() {
  analogWrite(right_pwm, 0);
  digitalWrite(right_dir_1, HIGH);
  digitalWrite(right_dir_2, LOW);
}

void rightWheel::moveLeft() {
    analogWrite(right_pwm, 220);
    digitalWrite(right_dir_1, HIGH);
    digitalWrite(right_dir_2, LOW);
}

void rightWheel::moveRight() {
  analogWrite(right_pwm, 220);
  digitalWrite(right_dir_1, LOW);
  digitalWrite(right_dir_2, HIGH);
}
