#include "ultrasonic.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "robot.h"
#include "Arduino.h"
#include "constant.h"

// Implementation of 'robot' class
robot::robot(ultrasonic f, ultrasonic r, ultrasonic l, leftWheel lw, rightWheel rw) 
: Front(f), Right(r), Left(l), LeftWheel(lw), RightWheel(rw) {}

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

  // Ultrasonic Sensor Test
  Serial.print("Front Sensor: ");
  Serial.println(Front.retrieveData());
  delay(1000);

  Serial.print("Right Sensor: ");
  Serial.println(Right.retrieveData());
  delay(1000);

  Serial.print("Left Sensor: ");
  Serial.println(Left.retrieveData());
  delay(1000);
}

void robot::solveMaze() {
  float frontData = Front.retrieveData();
  float rightData = Right.retrieveData();
  float leftData = Left.retrieveData();
  while(true) {
    if (frontData > DIST_THRESH && rightData < DIST_THRESH && leftData < DIST_THRESH) { // left & right have obstacles but front is clear
      LeftWheel.moveForward();
      RightWheel.moveForward(); 
      // Serial.println("Forward");
      // delay(100);
    }
    else if ( frontData < DIST_THRESH && rightData < DIST_THRESH && leftData < DIST_THRESH) { // obstacle infront of all 3 sides, reverse
      LeftWheel.moveReverse();
      RightWheel.moveReverse();
      // Serial.println("Reverse");
      delay(500);
      if((leftData) > (rightData)) { 
        LeftWheel.moveLeft();
        RightWheel.moveLeft();
        // Serial.println("Left");
        // delay(100); 
      }
      else { 
        LeftWheel.moveRight();
        RightWheel.moveRight();
        // Serial.println("Right"); 
        // delay(100);
      }
    }
    else if (frontData < DIST_THRESH && rightData < DIST_THRESH && leftData > DIST_THRESH) { // obstacle on right and front sides, turn left side 
      LeftWheel.moveLeft(); 
      RightWheel.moveLeft(); 
      // Serial.println("Left");
      // delay(100);
    }
    else if (frontData < DIST_THRESH && rightData > DIST_THRESH && leftData < DIST_THRESH) { // obstacle on left and front sides, turn right side
      LeftWheel.moveRight();
      RightWheel.moveRight();
      // Serial.println("Right");
      // delay(100);
    }
    else  if (frontData < DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { // obstacle on front side, turn left
      LeftWheel.moveLeft();
      LeftWheel.moveLeft(); 
    }
    else if (frontData > DIST_THRESH && rightData > DIST_THRESH && leftData < DIST_THRESH) { // obstacle on left side, forward
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Forward");
      // delay(100);
    }
    else if (frontData > DIST_THRESH && rightData < DIST_THRESH && leftData > DIST_THRESH) { // obstacle on right side, turn left & then forward
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Forward");
      // delay(100);
    }
    else { // no osbtacle ahead
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Forward");
      // delay(100);
    }
  }
}

void robot::initializeMaze() {
  // Mark destination at the centre with int 'END'
  Maze[6][6], Maze[6][7], Maze[7][6], Maze[7][7] = END;
  // Mark starting point at corner with int 'START'
  Maze[11][0] = START;
  visited[11][0] = 1;
}