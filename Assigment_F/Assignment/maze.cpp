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

  // // Ultrasonic Sensor Test
  // Serial.print("Front Sensor: ");
  // Serial.println(Front.retrieveData());
  // delay(1000);

  // Serial.print("Right Sensor: ");
  // Serial.println(Right.retrieveData());
  // delay(1000);

  // Serial.print("Left Sensor: ");
  // Serial.println(Left.retrieveData());
  // delay(1000);
}

void maze::solveMaze() {
  const int SIDE_DIST_THRESH = 5;
  const int FRONT_DIST_THRESH = 15;
  
  while(true) {
    Front.retrieveData();
    Left.retrieveData();
    Right.retrieveData();
    if (Front.data > FRONT_DIST_THRESH && Right.data < SIDE_DIST_THRESH && Left.data < SIDE_DIST_THRESH) { // left & right have obstacles but front is clear
      LeftWheel.moveForward();
      RightWheel.moveForward();
      Serial.print("Front: "); Serial.print(Front.data); Serial.print("    ");
      Serial.print("Left: "); Serial.print(Left.data); Serial.print("    ");
      Serial.print("Right: "); Serial.print(Right.data); Serial.print("    ");
      Serial.println("Movement : Forward");
      delay(100);
    }
    else if (Front.data < FRONT_DIST_THRESH && Right.data < SIDE_DIST_THRESH && Left.data < SIDE_DIST_THRESH) { // obstacle infront of all 3 sides, reverse
      LeftWheel.moveReverse();
      RightWheel.moveReverse();
      Serial.print("Front: "); Serial.print(Front.data); Serial.print("    ");
      Serial.print("Left: "); Serial.print(Left.data); Serial.print("    ");
      Serial.print("Right: "); Serial.print(Right.data); Serial.print("    ");
      Serial.println("Movement : Reverse");
      delay(100);
    }
    else if (Front.data < FRONT_DIST_THRESH && Right.data < SIDE_DIST_THRESH && Left.data > SIDE_DIST_THRESH) { // obstacle on right and front sides, turn left side 
      LeftWheel.moveLeft(); 
      RightWheel.moveLeft(); 
      Serial.print("Front: "); Serial.print(Front.data); Serial.print("    ");
      Serial.print("Left: "); Serial.print(Left.data); Serial.print("    ");
      Serial.print("Right: "); Serial.print(Right.data); Serial.print("    ");
      Serial.println("Movement : Left");
      delay(100);
    }
    else if (Front.data < FRONT_DIST_THRESH && Right.data > SIDE_DIST_THRESH && Left.data < SIDE_DIST_THRESH) { // obstacle on left and front sides, turn right side
      LeftWheel.moveRight();
      RightWheel.moveRight();
      Serial.print("Front: "); Serial.print(Front.data); Serial.print("    ");
      Serial.print("Left: "); Serial.print(Left.data); Serial.print("    ");
      Serial.print("Right: "); Serial.print(Right.data); Serial.print("    ");
      Serial.println("Movement : Right");
      delay(100);
    }
    else  if (Front.data < FRONT_DIST_THRESH && Right.data > SIDE_DIST_THRESH && Left.data > SIDE_DIST_THRESH) { // obstacle on front side, turn left
      LeftWheel.moveLeft();
      LeftWheel.moveLeft();
      Serial.print("Front: "); Serial.print(Front.data); Serial.print("    ");
      Serial.print("Left: "); Serial.print(Left.data); Serial.print("    ");
      Serial.print("Right: "); Serial.print(Right.data); Serial.print("    ");
      Serial.println("Movement : Left");
      delay(100); 
    }
    else if (Front.data > FRONT_DIST_THRESH && Right.data > SIDE_DIST_THRESH && Left.data < SIDE_DIST_THRESH) { // obstacle on left side, forward
      LeftWheel.moveForward();
      RightWheel.moveForward();
      Serial.print("Front: "); Serial.print(Front.data); Serial.print("    ");
      Serial.print("Left: "); Serial.print(Left.data); Serial.print("    ");
      Serial.print("Right: "); Serial.print(Right.data); Serial.print("    ");
      Serial.println("Movement : Forward");
      delay(100);
    }
    else if (Front.data > FRONT_DIST_THRESH && Right.data < SIDE_DIST_THRESH && Left.data > SIDE_DIST_THRESH) { // obstacle on right side, forward
      LeftWheel.moveForward();
      RightWheel.moveForward();
      Serial.print("Front: "); Serial.print(Front.data); Serial.print("    ");
      Serial.print("Left: "); Serial.print(Left.data); Serial.print("    ");
      Serial.print("Right: "); Serial.print(Right.data); Serial.print("    ");
      Serial.println("Movement : Forward");
      delay(100);
    }
    else { // no osbtacle ahead
      LeftWheel.moveForward();
      RightWheel.moveForward();
      Serial.print("Front: "); Serial.print(Front.data); Serial.print("    ");
      Serial.print("Left: "); Serial.print(Left.data); Serial.print("    ");
      Serial.print("Right: "); Serial.print(Right.data); Serial.print("    ");
      Serial.println("Movement : Forward");
      delay(100);
    }
  }
}
