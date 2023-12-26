#include <TimedAction.h>

#include "constant.h"
#include "ultrasonic.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "encoder.h"
#include "maze.h"

leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR_1, LEFT_DIR_2);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR_1, RIGHT_DIR_2);
ultrasonic frontSensor(FRONT_TRIG, FRONT_ECHO);
ultrasonic rightSensor(RIGHT_TRIG, RIGHT_ECHO);
ultrasonic leftSensor(LEFT_TRIG, LEFT_ECHO);
encoder leftEncoder(LEFT_PIN, LEFT_ENC);
encoder rightEncoder(RIGHT_PIN, RIGHT_ENC);

maze final(leftWheelObj, rightWheelObj, frontSensor, leftSensor, rightSensor, leftEncoder, rightEncoder);

void setup() { 
  Serial.begin(115200);
  leftWheelObj.declarePin();
  rightWheelObj.declarePin();
  frontSensor.declarePin();
  rightSensor.declarePin();
  leftSensor.declarePin();
  leftEncoder.declarePin();
  rightEncoder.declarePin();

  final.enableTask();
}

void loop() {
  // final.testFunctions();
  // final.solveMaze();

  // Serial.print("Left Encoder Pulse: "); Serial.print(leftPulse); Serial.print("    ");
  // Serial.print("Right Encoder Pulse:"); Serial.println(rightPulse);

  final.checkTask();  
}