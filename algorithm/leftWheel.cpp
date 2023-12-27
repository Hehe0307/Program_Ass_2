#include "wheel.h"
#include "leftWheel.h"
#include "Arduino.h"

// Implementation of 'leftWheel' class
leftWheel::leftWheel(const int left_pwm, const int left_dir_1, const int left_dir_2) 
: left_pwm(left_pwm), left_dir_1(left_dir_1), left_dir_2(left_dir_2) {}

void leftWheel::declarePin() {
  wheel::wheel(left_pwm, left_dir_1, left_dir_2);
}

void leftWheel::moveForward() {
  analogWrite(left_pwm, 220);
  digitalWrite(left_dir_1, HIGH);
  digitalWrite(left_dir_2, LOW);
}

void leftWheel::moveStop() {
  analogWrite(left_pwm, 0);
  digitalWrite(left_dir_1, HIGH);
  digitalWrite(left_dir_2, LOW);
}
 
void leftWheel::moveLeft() {
  analogWrite(left_pwm, 220);
  digitalWrite(left_dir_1, LOW);
  digitalWrite(left_dir_2, HIGH);
}

void leftWheel::moveRight() {
  analogWrite(left_pwm, 220);
  digitalWrite(left_dir_1, HIGH);
  digitalWrite(left_dir_2, LOW);
}
