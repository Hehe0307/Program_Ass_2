#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "IR.h"
#include "robot.h"
#include "Arduino.h"
#include "constant.h"

// Implementation of 'robot' class
robot::robot(leftWheel lw, rightWheel rw, IR fir, IR lir, IR rir) 
: LeftWheel(lw), RightWheel(rw), FrontIR(fir), LeftIR(lir), RightIR(rir) {}

void robot::testFunctions() {
  // Wheel Function Test
  Serial.println("Forward");
  LeftWheel.moveForward();
  RightWheel.moveForward();
  delay(2000);

  Serial.println("Left");
  LeftWheel.moveLeft();
  RightWheel.moveLeft();
  delay(2000);
  
  Serial.println("Right");
  LeftWheel.moveRight();
  RightWheel.moveRight();
  delay(2000);

  Serial.println("Reverse");
  LeftWheel.moveReverse();
  RightWheel.moveReverse();
  delay(2000);

  Serial.println("Stop");
  LeftWheel.moveStop();
  RightWheel.moveStop();
  delay(2000);
}

void robot::IRsolveMaze() {
  while(true) {
    FrontIR.retrieveData();
    LeftIR.retrieveData();
    RightIR.retrieveData();
    if (!FrontIR.status && LeftIR.status && RightIR.status) { // left & right have obstacles but front is clear, Forward
      LeftWheel.moveForward();
      RightWheel.moveForward(); 
      Serial.println("Forward");
      delay(100);
    }
    else if (FrontIR.status && LeftIR.status && RightIR.status) { // obstacle in front of all 3 sides, reverse
      LeftWheel.moveReverse();
      RightWheel.moveReverse();
      Serial.println("Backward");
      delay(100);
    }
    else if (FrontIR.status && !LeftIR.status && RightIR.status) { // obstacle on right and front sides, turn left side 
      LeftWheel.moveLeft(); 
      RightWheel.moveLeft(); 
      Serial.println("Left");
      delay(100);
    }
    else if (FrontIR.status && LeftIR.status && !RightIR.status) { // obstacle on left and front sides, turn right side
      LeftWheel.moveRight();
      RightWheel.moveRight();
      Serial.println("Right");
      delay(100);
    }
    else  if (FrontIR.status && !LeftIR.status && !RightIR.status) { // obstacle on front side, turn left
      LeftWheel.moveLeft();
      LeftWheel.moveLeft(); 
      Serial.println("Left");
      delay(100);
    }
    else if (!FrontIR.status && LeftIR.status && !RightIR.status) { // obstacle on left side, forward
      LeftWheel.moveForward();
      RightWheel.moveForward();
      Serial.println("Forward");
      delay(100);
    }
    else if (!FrontIR.status && !LeftIR.status && RightIR.status) { // obstacle on right side, forward
      LeftWheel.moveForward();
      RightWheel.moveForward();
      Serial.println("Forward");
      delay(100);
    }
    else { // no osbtacle ahead
      LeftWheel.moveForward();
      RightWheel.moveForward();
      Serial.println("Forward");
      delay(100);
    }
  }
}

void robot::forwardMovement() {
  LeftWheel.moveForward();
  RightWheel.moveForward();
}

void robot::leftMovement() {
  LeftWheel.moveLeft();
  RightWheel.moveLeft();
}

void robot::rightMovement() {
  LeftWheel.moveRight();
  RightWheel.moveRight();
}

void robot::backwardMovement() {
  LeftWheel.moveReverse();
  RightWheel.moveReverse();
}

void robot::stopMovement() {
  LeftWheel.moveStop();
  RightWheel.moveStop();
}

void robot::declareAllPins() {
  LeftWheel.declarePin();
  RightWheel.declarePin();
  FrontIR.declarePin();
  LeftIR.declarePin();
  RightIR.declarePin();
}