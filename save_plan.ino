#include "Arduino.h"

// Pin Declaration
#define LEFT_PWM 5
#define LEFT_DIR_1 4
#define LEFT_DIR_2 A0
#define RIGHT_PWM 6
#define RIGHT_DIR_1 7
#define RIGHT_DIR_2 A1

#define FRONT_TRIG 2
#define FRONT_ECHO 3
#define RIGHT_TRIG 8
#define RIGHT_ECHO 9
#define LEFT_TRIG 10
#define LEFT_ECHO 11

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_DIR_1, OUTPUT);
  pinMode(LEFT_DIR_2, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(RIGHT_DIR_1, OUTPUT);
  pinMode(RIGHT_DIR_2, OUTPUT);

  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);
  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);
}

void moveForward() {
  digitalWrite(LEFT_PWM, HIGH);
  digitalWrite(LEFT_DIR_1, HIGH);
  digitalWrite(LEFT_DIR_2, LOW);
  digitalWrite(RIGHT_PWM, HIGH);
  digitalWrite(RIGHT_DIR_1, HIGH);
  digitalWrite(RIGHT_DIR_2, LOW);
}

void moveLeft() {
  digitalWrite(LEFT_PWM, HIGH);
  digitalWrite(LEFT_DIR_1, LOW);
  digitalWrite(LEFT_DIR_2, HIGH);
  digitalWrite(RIGHT_PWM, HIGH);
  digitalWrite(RIGHT_DIR_1, HIGH);
  digitalWrite(RIGHT_DIR_2, LOW);
}

void moveRight() {
  digitalWrite(LEFT_PWM, HIGH);
  digitalWrite(LEFT_DIR_1, HIGH);
  digitalWrite(LEFT_DIR_2, LOW);
  digitalWrite(RIGHT_PWM, HIGH);
  digitalWrite(RIGHT_DIR_1, LOW);
  digitalWrite(RIGHT_DIR_2, HIGH);
}

void moveReverse() {
  digitalWrite(LEFT_PWM, HIGH);
  digitalWrite(LEFT_DIR_1, LOW);
  digitalWrite(LEFT_DIR_2, HIGH);
  digitalWrite(RIGHT_PWM, HIGH);
  digitalWrite(RIGHT_DIR_1, LOW);
  digitalWrite(RIGHT_DIR_2, HIGH);
}

void moveStop() {
  digitalWrite(LEFT_PWM, LOW);
  digitalWrite(LEFT_DIR_1, HIGH);
  digitalWrite(LEFT_DIR_2, LOW);
  digitalWrite(RIGHT_PWM, LOW);
  digitalWrite(RIGHT_DIR_1, HIGH);
  digitalWrite(RIGHT_DIR_2, LOW);
}

void loop() {
  moveForward();
  delay(2000);
  moveLeft();
  delay(2000);
  moveRight();
  delay(2000);
  moveReverse();
  delay(2000);
  moveStop();
  delay(2000);
}