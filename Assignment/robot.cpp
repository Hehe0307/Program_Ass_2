#include "ultrasonic.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "robot.h"
#include "Arduino.h"

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
  const int dist_thresh = 5;
  while (true) {
    // Your sensor readings and movement logic here
    // Use Front.retrieveData(), Right.retrieveData(), and Left.retrieveData() for sensor readings
    // Use LeftWheel.moveForward(), RightWheel.moveForward(), LeftWheel.turnLeft(), RightWheel.turnLeft(),
    // LeftWheel.turnRight(), RightWheel.turnRight() for robot movements

    if (Front.retrieveData() > dist_thresh && Right.retrieveData() > dist_thresh && Left.retrieveData() > dist_thresh) { // no obstacles ahead  
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Forward");
      // delay(100);
    }
    else if ( Front.retrieveData() > dist_thresh && Right.retrieveData() < dist_thresh && Left.retrieveData() < dist_thresh) { // left & right have obstacles but front is clear
      LeftWheel.moveForward();
      RightWheel.moveForward(); 
      // Serial.println("Forward");
      // delay(100);
    }
    else if ( Front.retrieveData() < dist_thresh && Right.retrieveData() < dist_thresh && Left.retrieveData() < dist_thresh) { // obstacle infront of all 3 sides, reverse
      LeftWheel.moveReverse();
      RightWheel.moveReverse();
      // Serial.println("Reverse");
      delay(500);
      if((Left.retrieveData()) > (Right.retrieveData())) { 
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
    else if (Front.retrieveData() < dist_thresh && Right.retrieveData() < dist_thresh && Left.retrieveData() > dist_thresh) { // obstacle on right and front sides, turn left side 
      LeftWheel.moveLeft(); 
      RightWheel.moveLeft(); 
      // Serial.println("Left");
      // delay(100);
    }
    else if (Front.retrieveData() < dist_thresh && Right.retrieveData() > dist_thresh && Left.retrieveData() < dist_thresh) { // obstacle on left and front sides, turn right side
      LeftWheel.moveRight();
      RightWheel.moveRight();
      // Serial.println("Right");
      // delay(100);
    }
    else  if (Front.retrieveData() < dist_thresh && Right.retrieveData() > dist_thresh && Left.retrieveData() > dist_thresh) { // obstacle on front side, turn left & then forward
      LeftWheel.moveLeft();
      LeftWheel.moveLeft(); 
      delay(500); 
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Left then Forward"); 
      // delay(100);
    }
    else if (Front.retrieveData() > dist_thresh && Right.retrieveData() > dist_thresh && Left.retrieveData() < dist_thresh) { // obstacle on left side, turn right & then forward
      LeftWheel.moveRight();
      RightWheel.moveRight();
      delay(500);
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Right then Forward");
      // delay(100);
    }
    else if (Front.retrieveData() > dist_thresh && Right.retrieveData() < dist_thresh && Left.retrieveData() > dist_thresh) { // obstacle on right side, turn left & then forward 
      LeftWheel.moveLeft();
      RightWheel.moveLeft();
      delay(500);
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Left then Forward");
      // delay(100);
    }
    else { 
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Forward");
      // delay(100);
    }
  }
}
