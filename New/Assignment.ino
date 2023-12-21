#include <TimedAction.h>

#include "constant.h"
#include "variable.h"
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

void checkMovementCode();
void executeMovementCode();

TimedAction checkMovement = TimedAction(10, checkMovementCode);
TimedAction executeMovement = TimedAction(10, executeMovementCode);

void checkMovementCode() {
  long frontData = frontSensor.retrieveData();
  long leftData = leftSensor.retrieveData();
  long rightData = rightSensor.retrieveData();

  if(frontData < dist_thresh && rightData < dist_thresh && leftData < dist_thresh) { direction = REVERSE; } // dead end
  else if(frontData > dist_thresh && rightData < dist_thresh && leftData < dist_thresh) { direction = FORWARD; } // obstacle at left & right
  else if(frontData < dist_thresh && rightData < dist_thresh && leftData > dist_thresh) { direction = LEFT; } // obstacle at front & right
  else if(frontData > dist_thresh && rightData > dist_thresh && leftData > dist_thresh) { direction = RIGHT; } // obstacle at front & left
  else if(frontData > dist_thresh && rightData > dist_thresh && leftData > dist_thresh) { direction = LEFT; } // obstacle at front
  else if(frontData > dist_thresh && rightData > dist_thresh && leftData > dist_thresh) { direction = FORWARD; } // obstacle at left
  else if(frontData > dist_thresh && rightData > dist_thresh && leftData > dist_thresh) { direction = FORWARD; } // obstacle at right
  else { direction = FORWARD; } // no obstacle ahead 
}

void executeMovementCode() {
  switch(direction) {
    case FORWARD: {
      leftWheelObj.moveForward();
      rightWheelObj.moveForward();
    }
    case LEFT: {
      leftWheelObj.moveLeft();
      rightWheelObj.moveLeft();
    }
    case RIGHT: {
      leftWheelObj.moveRight();
      rightWheelObj.moveRight();
    }
    case REVERSE: {
      leftWheelObj.moveReverse();
      rightWheelObj.moveReverse();
    }
    default:
      leftWheelObj.moveStop();
      rightWheelObj.moveStop();
  }
}

void setup() { 
  Serial.begin(115200);
  leftWheelObj.declarePin();
  rightWheelObj.declarePin();
  frontSensor.declarePin();
  rightSensor.declarePin();
  leftSensor.declarePin();
  // checkMovement.enable();
  // executeMovement.enable();
}

void loop() {
  // myRobot.testFunctions();
  myRobot.solveMaze();
  // checkMovement.check();
  // executeMovement.check();
}