#include <TimedAction.h>

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

void GetLeftPulseTaskCode();
void GetRightPulseTaskCode();
void checkMovementCode();
void executeMovementCode();
void updateMazeCode();

TimedAction GetLeftPulseTask = TimedAction(TASK_INTERVAL, GetLeftPulseTaskCode);
TimedAction GetRightPulseTask = TimedAction(TASK_INTERVAL, GetRightPulseTaskCode);
TimedAction checkMovement = TimedAction(TASK_INTERVAL, checkMovementCode);
TimedAction executeMovement = TimedAction(TASK_INTERVAL, executeMovementCode);
TimedAction updateMaze = TimedAction(TASK_INTERVAL, updateMazeCode);

volatile uint32_t leftPulse = 0;
volatile uint32_t rightPulse = 0;
int row, col = 0;

void GetLeftPulseTaskCode() { cli(); sei(); }
void GetRightPulseTaskCode() { cli(); sei(); }
void leftCounter() { leftPulse++; }
void rightCounter() { rightPulse++; }

void updateMazeCode() {
  switch(movement) {
    case FORWARD: {
      switch(direction) {
        case NORTH:
          { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { row--; break; } }
        case SOUTH:
          { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { row++; break; } }
        case WEST:
          { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { col--; break; } }
        case EAST:
          { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { col++; break; } }
      }
    }
    case LEFT:
    case RIGHT:
    case REVERSE:
      { break; }
  }
  Serial.print(row); Serial.print("     "); Serial.println(col); 
}

void checkMovementCode() {
  long frontData = frontSensor.retrieveData();
  long rightData = rightSensor.retrieveData();
  long leftData = leftSensor.retrieveData();
  
  if(frontData < DIST_THRESH && rightData < DIST_THRESH && leftData < DIST_THRESH) { movement = REVERSE; myRobot.Maze[row][col] = WALL; } // dead end
  else if(frontData > DIST_THRESH && rightData < DIST_THRESH && leftData < DIST_THRESH) { movement = FORWARD; } // obstacle at left & right
  else if(frontData < DIST_THRESH && rightData < DIST_THRESH && leftData > DIST_THRESH) { movement = LEFT; } // obstacle at front & right
  else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { movement = RIGHT; } // obstacle at front & left
  else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { movement = LEFT; } // obstacle at front
  else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { movement = FORWARD; } // obstacle at left
  else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { movement = FORWARD; } // obstacle at right
  else { movement = FORWARD; } // no obstacle ahead 
}

void executeMovementCode() {
  if(myRobot.Maze[row][col] != WALL && myRobot.Maze[row][col] != START && myRobot.Maze[row][col] != END) {
    switch(movement) {
      case FORWARD:
        leftWheelObj.moveForward();
        rightWheelObj.moveForward();
        break;
      case LEFT: 
        leftWheelObj.moveLeft();
        rightWheelObj.moveLeft();
        if(direction == NORTH) { direction = WEST; }
        else if(direction == SOUTH) { direction = EAST; }
        else if(direction == WEST) { direction = SOUTH; }
        else { direction = NORTH; }
        break;
      case RIGHT:
        leftWheelObj.moveRight();
        rightWheelObj.moveRight();
        if(direction == NORTH) { direction = EAST; }
        else if(direction == SOUTH) { direction = WEST; }
        else if(direction == WEST) { direction = NORTH; }
        else { direction = SOUTH; }
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
  pinMode(digitalPinToInterrupt(LEFT_ENC), INPUT);
  pinMode(digitalPinToInterrupt(RIGHT_ENC), INPUT);

  GetLeftPulseTask.enable();
  GetRightPulseTask.enable();
  checkMovement.enable();
  executeMovement.enable();
  updateMaze.enable();
}

void loop() {
  // myRobot.testFunctions();
  // myRobot.solveMaze();

  Serial.print("Left Encoder Pulse: "); Serial.print(leftPulse); Serial.print("    ");
  Serial.print("Right Encoder Pulse:"); Serial.println(rightPulse);

  GetLeftPulseTask.check();
  GetRightPulseTask.check();
  checkMovement.check();
  executeMovement.check();
  updateMaze.check();
}