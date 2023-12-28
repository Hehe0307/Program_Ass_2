#include <TimedAction.h>

#include "constant.h"
#include "ultrasonic.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "robot.h"

leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR_1, LEFT_DIR_2, 200);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR_1, RIGHT_DIR_2, 200);
ultrasonic frontSensor(FRONT_TRIG, FRONT_ECHO);
ultrasonic rightSensor(RIGHT_TRIG, RIGHT_ECHO);
ultrasonic leftSensor(LEFT_TRIG, LEFT_ECHO);
robot myRobot(frontSensor, rightSensor, leftSensor, leftWheelObj, rightWheelObj);

void GetLeftPulseTaskCode();
void GetRightPulseTaskCode();
void checkMovementCode();
void executeMovementCode();
void updateMazeCode();
void frontDetectCode();
void leftDetectCode();
void rightDetectCode();

TimedAction GetLeftPulseTask = TimedAction(TASK_INTERVAL, GetLeftPulseTaskCode);
TimedAction GetRightPulseTask = TimedAction(TASK_INTERVAL, GetRightPulseTaskCode);
TimedAction checkMovement = TimedAction(TASK_INTERVAL, checkMovementCode);
TimedAction executeMovement = TimedAction(TASK_INTERVAL, executeMovementCode);
TimedAction updateMaze = TimedAction(TASK_INTERVAL, updateMazeCode);
TimedAction frontDetect = TimedAction(TASK_INTERVAL, frontDetectCode);
TimedAction leftDetect = TimedAction(TASK_INTERVAL, leftDetectCode);
TimedAction rightDetect = TimedAction(TASK_INTERVAL, rightDetectCode);

volatile uint32_t leftPulse = 0;
volatile uint32_t rightPulse = 0;
int row, col = 0;

void GetLeftPulseTaskCode() { cli(); sei(); }
void leftCounter() { leftPulse++; }

void GetRightPulseTaskCode() { cli(); sei(); }
void rightCounter() { rightPulse++; }

void frontDetectCode() { frontSensor.retrieveData(); }

void leftDetectCode() { leftSensor.retrieveData(); }

void rightDetectCode() { rightSensor.retrieveData(); }

void updateMazeCode() {
  switch(movement) {
    case FORWARD: {
      switch(direction) {
        case NORTH:
          { if(leftPulse % PULSE_PER_GRID == 0) { row--; myRobot.visited[row][col] = 1; break; } }
        case SOUTH:
          { if(leftPulse % PULSE_PER_GRID == 0) { row++; myRobot.visited[row][col] = 1; break; } }
        case WEST:
          { if(leftPulse % PULSE_PER_GRID == 0) { col--; myRobot.visited[row][col] = 1; break; } }
        case EAST:
          { if(leftPulse % PULSE_PER_GRID == 0) { col++; myRobot.visited[row][col] = 1; break; } }
      }
    }
    case LEFT:
    case RIGHT:
    case REVERSE:
      { break; }
  }
  Serial.print("Row: "); Serial.print(row); Serial.print("     "); Serial.print("Col: "); Serial.println(col); 
}

void checkMovementCode() {
  if(frontSensor.data < DIST_THRESH && rightSensor.data < DIST_THRESH && leftSensor.data < DIST_THRESH) { // dead end
    movement = REVERSE; 
    myRobot.Maze[row][col] = WALL; 
    switch(direction) {
      case NORTH: { myRobot.Maze[row+1][col] = 0; }
      case SOUTH: { myRobot.Maze[row-1][col] = 0; }
      case EAST: { myRobot.Maze[row][col-1] = 0; }
      case WEST: { myRobot.Maze[row][col+1] = 0; }
    }
  } 
  else if(frontSensor.data > DIST_THRESH && rightSensor.data < DIST_THRESH && leftSensor.data < DIST_THRESH) { movement = FORWARD; } // obstacle at left & right
  else if(frontSensor.data < DIST_THRESH && rightSensor.data < DIST_THRESH && leftSensor.data > DIST_THRESH) { movement = LEFT; } // obstacle at front & right
  else if(frontSensor.data > DIST_THRESH && rightSensor.data > DIST_THRESH && leftSensor.data > DIST_THRESH) { movement = RIGHT; } // obstacle at front & left
  else if(frontSensor.data > DIST_THRESH && rightSensor.data > DIST_THRESH && leftSensor.data > DIST_THRESH) { movement = LEFT; } // obstacle at front
  else if(frontSensor.data > DIST_THRESH && rightSensor.data > DIST_THRESH && leftSensor.data > DIST_THRESH) { movement = FORWARD; } // obstacle at left
  else if(frontSensor.data > DIST_THRESH && rightSensor.data > DIST_THRESH && leftSensor.data > DIST_THRESH) { movement = FORWARD; } // obstacle at right
  else { movement = FORWARD; } // no obstacle ahead 
}

void executeMovementCode() {
  if(myRobot.Maze[row][col] != WALL && myRobot.Maze[row][col] != END) {
    switch(movement) {
      case FORWARD:
        leftWheelObj.moveForward();
        rightWheelObj.moveForward();
        Serial.println("Forward");
        break;
      case LEFT: 
        // checkMovement.disable();
        // leftPulse = 0;
        // rightPulse = 0;
        // while(leftPulse < 11 && rightPulse < 11) {
          leftWheelObj.moveLeft();
          rightWheelObj.moveLeft();
          Serial.println("Left");
        // }
        // leftPulse = 0;
        // rightPulse = 0;        
        if(direction == NORTH) { direction = WEST; }
        else if(direction == SOUTH) { direction = EAST; }
        else if(direction == WEST) { direction = SOUTH; }
        else { direction = NORTH; }
        // checkMovement.enable();
        break;
      case RIGHT:
        // checkMovement.disable();
        // leftPulse = 0;
        // rightPulse = 0;
        // while (leftPulse < 11 && rightPulse < 11) {
          leftWheelObj.moveRight();
          rightWheelObj.moveRight();
        // }
        // leftPulse = 0;
        // rightPulse = 0;
        if(direction == NORTH) { direction = EAST; }
        else if(direction == SOUTH) { direction = WEST; }
        else if(direction == WEST) { direction = NORTH; }
        else { direction = SOUTH; }
        // checkMovement.enable();
        break;
      case REVERSE: 
        leftWheelObj.moveReverse();
        rightWheelObj.moveReverse();
        if(direction == NORTH) { direction = SOUTH; }
        else if(direction == SOUTH) { direction = NORTH; }
        else if(direction == WEST) { direction = EAST; }
        else { direction = WEST; }
        break;
      default:
        leftWheelObj.moveStop();
        rightWheelObj.moveStop();
        break;
    }
  }
  if(myRobot.Maze[row][col] == END) {
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
  pinMode(LEFT_ENC, INPUT);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC), leftCounter, RISING);
  // pinMode(RIGHT_ENC, INPUT);
  // attachInterrupt(digitalPinToInterrupt(RIGHT_ENC), rightCounter, RISING);

  GetLeftPulseTask.enable();
  GetRightPulseTask.enable();
  checkMovement.enable();
  executeMovement.enable();
  updateMaze.enable();
  frontDetect.enable();
  leftDetect.enable();
  rightDetect.enable();
}

void loop() {
  // Serial.print("Left Encoder Pulse: "); Serial.print(leftPulse); Serial.print("    ");
  // Serial.print("Right Encoder Pulse:"); Serial.println(rightPulse);

  GetLeftPulseTask.check();
  GetRightPulseTask.check();
  checkMovement.check();
  executeMovement.check();
  updateMaze.check();
  frontDetect.check();
  leftDetect.check();
  rightDetect.check();
}