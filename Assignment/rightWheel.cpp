#include "wheel.h"
#include "rightWheel.h"
#include "Arduino.h"

// Implementation of 'rightWheel' class
rightWheel::rightWheel(int right_pwm, int right_dir_1, int right_dir_2) 
: right_pwm(right_pwm), right_dir_1(right_dir_1), right_dir_2(right_dir_2) {}

void rightWheel::declarePin() {
  pinMode(right_pwm, OUTPUT);
  pinMode(right_dir_1, OUTPUT);
  pinMode(right_dir_2, OUTPUT);
}

void rightWheel::moveForward() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir_1, HIGH);
  digitalWrite(right_dir_2, LOW);
}

void rightWheel::reverse() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir_1, LOW);
  digitalWrite(right_dir_2, HIGH);
}

void rightWheel::stop() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir_1, HIGH);
  digitalWrite(right_dir_2, LOW);
}

void rightWheel::turnLeft() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir_1, HIGH);
  digitalWrite(right_dir_2, LOW);
}

void rightWheel::turnRight() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir_1, LOW);
  digitalWrite(right_dir_2, HIGH);
}