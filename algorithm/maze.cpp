#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "ultrasonic.h"
#include "maze.h"
#include "queue.h"

#include "Arduino.h"

maze::maze(leftWheel LeftWheel, rightWheel RightWheel, ultrasonic Front, ultrasonic Left, ultrasonic Right)
: LeftWheel(LeftWheel), RightWheel(RightWheel), Front(Front), Left(Left), Right(Right) {}

void maze::testFunctions() {
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
  LeftWheel.moveRight();
  RightWheel.moveRight();
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

void maze::solveMaze() {
  float frontData = Front.retrieveData();
  float rightData = Right.retrieveData();
  float leftData = Left.retrieveData();
  const int SIDE_DIST_THRESH = 7;
  const int FRONT_DIST_THRESH = 12;
  while(true) {
    if (frontData > FRONT_DIST_THRESH && rightData < SIDE_DIST_THRESH && leftData < SIDE_DIST_THRESH) { // left & right have obstacles but front is clear
      LeftWheel.moveForward();
      RightWheel.moveForward(); 
      // Serial.println("Forward");
      // delay(100);
    }
    else if ( frontData < FRONT_DIST_THRESH && rightData < SIDE_DIST_THRESH && leftData < SIDE_DIST_THRESH) { // obstacle infront of all 3 sides, reverse
      LeftWheel.moveRight();
      RightWheel.moveRight();
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
    else if (frontData < FRONT_DIST_THRESH && rightData < SIDE_DIST_THRESH && leftData > SIDE_DIST_THRESH) { // obstacle on right and front sides, turn left side 
      LeftWheel.moveLeft(); 
      RightWheel.moveLeft(); 
      // Serial.println("Left");
      // delay(100);
    }
    else if (frontData < FRONT_DIST_THRESH && rightData > SIDE_DIST_THRESH && leftData < SIDE_DIST_THRESH) { // obstacle on left and front sides, turn right side
      LeftWheel.moveRight();
      RightWheel.moveRight();
      // Serial.println("Right");
      // delay(100);
    }
    else  if (frontData < FRONT_DIST_THRESH && rightData > SIDE_DIST_THRESH && leftData > SIDE_DIST_THRESH) { // obstacle on front side, turn left
      LeftWheel.moveLeft();
      LeftWheel.moveLeft(); 
    }
    else if (frontData > FRONT_DIST_THRESH && rightData > SIDE_DIST_THRESH && leftData < SIDE_DIST_THRESH) { // obstacle on left side, forward
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Forward");
      // delay(100);
    }
    else if (frontData > FRONT_DIST_THRESH && rightData < SIDE_DIST_THRESH && leftData > SIDE_DIST_THRESH) { // obstacle on right side, turn left & then forward
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
