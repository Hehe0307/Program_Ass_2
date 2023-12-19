#include "constant.h"

#include "ultrasonic.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "robot.h"

leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR1, LEFT_DIR2);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR1, RIGHT_DIR2);
ultrasonic frontSensor(FRONT_TRIG, FRONT_ECHO);
ultrasonic rightSensor(RIGHT_TRIG, RIGHT_ECHO);
ultrasonic leftSensor(LEFT_TRIG, LEFT_ECHO);
robot myRobot(frontSensor, rightSensor, leftSensor, leftWheelObj, rightWheelObj);

void setup() { 
  Serial.begin(115200);
}

void loop()
{
  Serial.println("Start");
  leftWheelObj.moveForward();
  leftWheelObj.moveForward();
  Serial.println("End");
}