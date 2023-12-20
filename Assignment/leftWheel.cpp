#include "wheel.h"
#include "leftWheel.h"
#include "Arduino.h"

// Implementation of 'leftWheel' class
leftWheel::leftWheel(int left_pwm, int left_dir_1, int left_dir_2) 
: left_pwm(left_pwm), left_dir_1(left_dir_1), left_dir_2(left_dir_2) {}

void leftWheel::declarePin() {
  pinMode(left_pwm, OUTPUT);
  pinMode(left_dir_1, OUTPUT);
  pinMode(left_dir_2, OUTPUT);
}

void leftWheel::moveForward() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir_1, HIGH);
  digitalWrite(left_dir_2, LOW);
}

void leftWheel::moveReverse() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir_1, LOW);
  digitalWrite(left_dir_2, HIGH);
}

void leftWheel::moveStop() {
  digitalWrite(left_pwm, LOW);
  digitalWrite(left_dir_1, HIGH);
  digitalWrite(left_dir_2, LOW);
}

void leftWheel::moveLeft() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir_1, LOW);
  digitalWrite(left_dir_2, HIGH);
}

void leftWheel::moveRight() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir_1, HIGH);
  digitalWrite(left_dir_2, LOW);
}