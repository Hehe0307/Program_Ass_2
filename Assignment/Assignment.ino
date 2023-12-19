#include "constant.h"

#include "ultrasonic.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "robot.h"

leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR_1, LEFT_DIR_2);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR_1, RIGHT_DIR_2);
ultrasonic frontSensor(FRONT_TRIG, FRONT_ECHO);
ultrasonic rightSensor(RIGHT_TRIG, RIGHT_ECHO);
ultrasonic leftSensor(LEFT_TRIG, LEFT_ECHO);
robot myRobot(frontSensor, rightSensor, leftSensor, leftWheelObj, rightWheelObj);

void setup() { 
  Serial.begin(115200);
  leftWheelObj.declarePin();
  rightWheelObj.declarePin();
  frontSensor.declarePin();
  rightSensor.declarePin();
  leftSensor.declarePin();
}

void loop()
{
  Serial.println("Start");
  
  // Wheel Function Test
  Serial.println("Forward");
  leftWheelObj.moveForward();
  rightWheelObj.moveForward();
  delay(1000);
  leftWheelObj.stop();
  rightWheelObj.stop();
  delay(1000);

  Serial.println("Left");
  leftWheelObj.turnLeft();
  rightWheelObj.turnLeft();
  delay(1000);
  leftWheelObj.stop();
  rightWheelObj.stop();
  
  Serial.println("Right");
  leftWheelObj.turnRight();
  rightWheelObj.turnRight();
  delay(1000);
  leftWheelObj.stop();
  rightWheelObj.stop();
  delay(1000);

  Serial.println("Reverse");
  leftWheelObj.reverse();
  rightWheelObj.reverse();
  delay(1000);
  leftWheelObj.stop();
  rightWheelObj.stop();
  delay(1000);

  // Ultrasonic Sensor Test
  Serial.print("Front Sensor: ");
  Serial.println(frontSensor.retrieveData());
  delay(1000);

  Serial.print("Right Sensor: ");
  Serial.println(rightSensor.retrieveData());
  delay(1000);

  Serial.print("Left Sensor: ");
  Serial.println(leftSensor.retrieveData());
  delay(1000);

  Serial.println("End");
}