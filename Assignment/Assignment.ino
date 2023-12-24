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
      // For Maze
      switch(direction) {
        case NORTH:
          { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { row--; myRobot.visited[row][col] = 1; break; } }
        case SOUTH:
          { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { row++; myRobot.visited[row][col] = 1; break; } }
        case WEST:
          { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { col--; myRobot.visited[row][col] = 1; break; } }
        case EAST:
          { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { col++; myRobot.visited[row][col] = 1; break; } }
      }

      // For NewMaze
      // switch(direction) {
      //   case NORTH:
      //     { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { row-=2; myRobot.NewVisited[row][col] = 1; break; } }
      //   case SOUTH:
      //     { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { row+=2; myRobot.NewVisited[row][col] = 1; break; } }
      //   case WEST:
      //     { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { col-=2; myRobot.NewVisited[row][col] = 1; break; } }
      //   case EAST:
      //     { if(leftPulse == rightPulse && leftPulse % PULSE_PER_GRID == 0) { col+=2; myRobot.NewVisited[row][col] = 1; break; } }
      // }
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
  
  // Maze
  if(frontData < DIST_THRESH && rightData < DIST_THRESH && leftData < DIST_THRESH) { movement = REVERSE; myRobot.Maze[row][col] = WALL; } // dead end
  else if(frontData > DIST_THRESH && rightData < DIST_THRESH && leftData < DIST_THRESH) { movement = FORWARD; } // obstacle at left & right
  else if(frontData < DIST_THRESH && rightData < DIST_THRESH && leftData > DIST_THRESH) { movement = LEFT; } // obstacle at front & right
  else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { movement = RIGHT; } // obstacle at front & left
  else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { movement = LEFT; } // obstacle at front
  else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { movement = FORWARD; } // obstacle at left
  else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { movement = FORWARD; } // obstacle at right
  else { movement = FORWARD; } // no obstacle ahead 

  // NewMaze
  // if(frontData < DIST_THRESH && rightData < DIST_THRESH && leftData < DIST_THRESH) { // dead end
  //   movement = REVERSE; 
  //   switch(direction) {
  //     case NORTH:
  //       myRobot.NewVisited[row-1][col] = 1;
  //       myRobot.NewVisited[row][col-1] = 1;
  //       myRobot.NewVisited[row][col+1] = 1;
  //     case SOUTH:
  //       myRobot.NewVisited[row+1][col] = 1;
  //       myRobot.NewVisited[row][col-1] = 1;
  //       myRobot.NewVisited[row][col+1] = 1;
  //     case EAST:
  //       myRobot.NewVisited[row-1][col] = 1;
  //       myRobot.NewVisited[row+1][col] = 1;
  //       myRobot.NewVisited[row][col+1] = 1;
  //     case WEST:
  //       myRobot.NewVisited[row-1][col] = 1;
  //       myRobot.NewVisited[row+1][col] = 1;
  //       myRobot.NewVisited[row][col-1] = 1;
  //   } 
  // }
  // else if(frontData > DIST_THRESH && rightData < DIST_THRESH && leftData < DIST_THRESH) { // obstacle at left & right
  //   movement = FORWARD;
  //   switch(direction) {
  //     case NORTH:
  //     case SOUTH: 
  //       myRobot.NewVisited[row][col-1] = 1;
  //       myRobot.NewVisited[row][col+1] = 1;
  //     case EAST:
  //     case WEST:
  //       myRobot.NewVisited[row-1][col] = 1;
  //       myRobot.NewVisited[row+1][col] = 1;
  //   }
  // } 
  // else if(frontData < DIST_THRESH && rightData < DIST_THRESH && leftData > DIST_THRESH) { // obstacle at front & right
  //   movement = LEFT; 
  //   switch(direction) {
  //     case NORTH:
  //       myRobot.NewVisited[row-1][col] = 1;
  //       myRobot.NewVisited[row][col+1] = 1;
  //     case SOUTH:
  //       myRobot.NewVisited[row+1][col] = 1;
  //       myRobot.NewVisited[row][col-1] = 1;
  //     case EAST:
  //       myRobot.NewVisited[row+1][col] = 1;
  //       myRobot.NewVisited[row][col+1] = 1;
  //     case WEST:
  //       myRobot.NewVisited[row-1][col] = 1;
  //       myRobot.NewVisited[row][col-1] = 1;
  //   }
  // }
  // else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { // obstacle at front & left 
  //   movement = RIGHT; 
  //   switch(direction) {
  //     case NORTH:
  //       myRobot.NewVisited[row-1][col] = 1;
  //       myRobot.NewVisited[row][col-1] = 1;
  //     case SOUTH:
  //       myRobot.NewVisited[row+1][col] = 1;
  //       myRobot.NewVisited[row][col+1] = 1;
  //     case EAST:
  //       myRobot.NewVisited[row-1][col] = 1;
  //       myRobot.NewVisited[row][col+1] = 1;
  //     case WEST:
  //       myRobot.NewVisited[row+1][col] = 1;
  //       myRobot.NewVisited[row][col-1] = 1;
  //   }
  // } 
  // else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { // obstacle at front
  //   movement = LEFT; 
  //   switch(direction) {
  //     case NORTH:
  //       myRobot.NewVisited[row-1][col] = 1;
  //     case SOUTH:
  //       myRobot.NewVisited[row+1][col] = 1;
  //     case EAST:
  //       myRobot.NewVisited[row][col+1] = 1;
  //     case WEST:
  //       myRobot.NewVisited[row][col-1] = 1;
  //   }
  // } 
  // else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { // obstacle at left
  //   movement = FORWARD;
  //   switch(direction) {
  //     case NORTH:
  //       myRobot.NewVisited[row][col-1] = 1;
  //     case SOUTH:
  //       myRobot.NewVisited[row+1][col+1] = 1;
  //     case EAST:
  //       myRobot.NewVisited[row-1][col] = 1;
  //     case WEST:
  //       myRobot.NewVisited[row+1][col] = 1;
  //   } 
  // }
  // else if(frontData > DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { // obstacle at right 
  //   movement = FORWARD; 
  //   switch(direction) {
  //     case NORTH:
  //       myRobot.NewVisited[row][col+1] = 1;
  //     case SOUTH:
  //       myRobot.NewVisited[row+1][col-1] = 1;
  //     case EAST:
  //       myRobot.NewVisited[row+1][col] = 1;
  //     case WEST:
  //       myRobot.NewVisited[row-1][col] = 1;
  // } 
  // } 
  // else { movement = FORWARD; } // no obstacle ahead
}

void executeMovementCode() {
  // Maze
  if(myRobot.Maze[row][col] != WALL && myRobot.Maze[row][col] != END) {
  // NewMaze
  // if(myRobot.NewMaze[row][col] != WALL && myRobot.NewMaze[row][col] != START && myRobot.NewMaze[row][col] != END) {
    switch(movement) {
      case FORWARD:
        leftWheelObj.moveForward();
        rightWheelObj.moveForward();
        break;
      case LEFT: 
        checkMovement.disable();
        leftPulse = 0;
        rightPulse = 0;
        while ( leftPulse < 11 && rightPulse < 11 )
        {
          leftWheelObj.moveLeft();
          rightWheelObj.moveLeft();
        }
        leftPulse = 0;
        rightPulse = 0;        
        if(direction == NORTH) { direction = WEST; }
        else if(direction == SOUTH) { direction = EAST; }
        else if(direction == WEST) { direction = SOUTH; }
        else { direction = NORTH; }
        checkMovement.enable();
        break;
      case RIGHT:
        checkMovement.disable();
        leftPulse = 0;
        rightPulse = 0;
        while ( leftPulse < 11 && rightPulse < 11 )
        {
          leftWheelObj.moveRight();
          rightWheelObj.moveRight();
        }
        leftPulse = 0;
        rightPulse = 0;
        if(direction == NORTH) { direction = EAST; }
        else if(direction == SOUTH) { direction = WEST; }
        else if(direction == WEST) { direction = NORTH; }
        else { direction = SOUTH; }
        checkMovement.enable();
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
  // Maze
  if(myRobot.Maze[row][col] == END) {
  // New Maze
  // if(myRobot.NewMaze[row][col] == END) {
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
  pinMode(RIGHT_ENC, INPUT);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC), leftCounter, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC), rightCounter, RISING);

  GetLeftPulseTask.enable();
  GetRightPulseTask.enable();
  checkMovement.enable();
  executeMovement.enable();
  updateMaze.enable();
}

void loop() {
  // myRobot.testFunctions();
  // myRobot.solveMaze();

  // Serial.print("Left Encoder Pulse: "); Serial.print(leftPulse); Serial.print("    ");
  // Serial.print("Right Encoder Pulse:"); Serial.println(rightPulse);

  GetLeftPulseTask.check();
  GetRightPulseTask.check();
  checkMovement.check();
  executeMovement.check();
  updateMaze.check();
}