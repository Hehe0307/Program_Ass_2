#include "ultrasonic.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "robot.h"
#include "Arduino.h"
#include "constant.h"
#include "variable.h"

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
  while (true) {
    frontData = Front.retrieveData(); 
    leftData = Left.retrieveData();
    rightData = Right.retrieveData();
    Serial.print(Front.retrieveData()); Serial.print("     "); Serial.print(Left.retrieveData()); Serial.print("     "); Serial.println(Right.retrieveData());
    if (frontData > dist_thresh && rightData < dist_thresh && leftData < dist_thresh) { // left & right have obstacles but front is clear
      LeftWheel.moveForward();
      RightWheel.moveForward(); 
      Serial.println("left & right have obstacles but front is clear");
      Serial.println("Forward");
    }
    else if (frontData < dist_thresh && rightData < dist_thresh && leftData < dist_thresh) { // obstacle infront of all 3 sides, reverse
      LeftWheel.moveReverse();
      RightWheel.moveReverse();
      Serial.println("obstacle infront of all 3 sides");
      Serial.println("Reverse");
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
    else if (frontData < dist_thresh && rightData < dist_thresh && leftData > dist_thresh) { // obstacle on right and front sides, turn left side 
      LeftWheel.moveLeft(); 
      RightWheel.moveLeft(); 
      Serial.println("obstacle on right and front sides, turn left side");
      Serial.println("Left");
    }
    else if (frontData < dist_thresh && rightData > dist_thresh && leftData < dist_thresh) { // obstacle on left and front sides, turn right side
      LeftWheel.moveRight();
      RightWheel.moveRight();
      Serial.println("obstacle on left and front sides, turn right side");
      Serial.println("Right");
    }
    else  if (frontData < dist_thresh && rightData > dist_thresh && leftData > dist_thresh) { // obstacle on front side, turn left & then forward
      LeftWheel.moveLeft();
      LeftWheel.moveLeft(); 
      Serial.println("obstacle on front side");
      Serial.println("Left");
      // delay(500); 
      // LeftWheel.moveForward();
      // RightWheel.moveForward();
      // Serial.println("Left then Forward"); 
      // delay(100);
    }
    else if (frontData > dist_thresh && rightData > dist_thresh && leftData < dist_thresh) { // obstacle on left side, turn right & then forward
      LeftWheel.moveForward();
      RightWheel.moveForward();
      Serial.println("obstacle on left side");
      Serial.println("Forward");
      // delay(500);
      // LeftWheel.moveForward();
      // RightWheel.moveForward();
      // Serial.println("Right then Forward");
      // delay(100);
    }
    else if (frontData > dist_thresh && rightData < dist_thresh && leftData > dist_thresh) { // obstacle on right side, turn left & then forward 
      LeftWheel.moveForward();
      RightWheel.moveForward();
      Serial.println("obstacle on right side");
      Serial.println("Forward");
      // delay(500);
      // LeftWheel.moveForward();
      // RightWheel.moveForward();
      // Serial.println("Left then Forward");
      // delay(100);
    }
    else { // no obstacle ahead
      LeftWheel.moveForward();
      RightWheel.moveForward();
      Serial.println("Clear");
      Serial.println("Forward");
      // Serial.println("Forward");
      // delay(100);
    }
  }
}
