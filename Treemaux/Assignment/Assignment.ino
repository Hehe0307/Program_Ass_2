//Group 10
//Members: Eng Chee Wen 22050762/1  Lee Yi Jia 22004570/1  Lai Zhe Jun S2182269/1  Chong Wei Jie 22054152/1

#include <TimedAction.h>
#include "constant.h"
#include "ultrasonic.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "IR.h"
#include "robot.h"

leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR_1, LEFT_DIR_2, 130);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR_1, RIGHT_DIR_2, 130);
ultrasonic frontSensor(FRONT_TRIG, FRONT_ECHO);
ultrasonic rightSensor(RIGHT_TRIG, RIGHT_ECHO);
ultrasonic leftSensor(LEFT_TRIG, LEFT_ECHO);
IR IRFront(FRONT_IR);
IR IRLeft(LEFT_IR);
IR IRRight(RIGHT_IR);
robot myRobot(frontSensor, rightSensor, leftSensor, leftWheelObj, rightWheelObj, IRFront, IRLeft, IRRight);

void GetLeftPulseTaskCode();
void GetRightPulseTaskCode();
void checkMovementCode();
void executeMovementCode();
void updateMazeCode();
// void frontDetectCode();
// void leftDetectCode();
// void rightDetectCode();
void frontIRCode();
void leftIRCode();
void rightIRCode();
void mazeMappingCode();
void getFrontGrid();
void getLeftGrid();
void getRightGrid();

TimedAction GetLeftPulseTask = TimedAction(1, GetLeftPulseTaskCode);
TimedAction GetRightPulseTask = TimedAction(TASK_INTERVAL, GetRightPulseTaskCode);
TimedAction checkMovement = TimedAction(TASK_INTERVAL, checkMovementCode);
TimedAction executeMovement = TimedAction(1000, executeMovementCode);
TimedAction updateMaze = TimedAction(TASK_INTERVAL, updateMazeCode);
// TimedAction frontDetect = TimedAction(TASK_INTERVAL, frontDetectCode);
// TimedAction leftDetect = TimedAction(TASK_INTERVAL, leftDetectCode);
// TimedAction rightDetect = TimedAction(TASK_INTERVAL, rightDetectCode);

TimedAction frontIRDetect = TimedAction(TASK_INTERVAL, frontIRCode);
TimedAction leftIRDetect = TimedAction(TASK_INTERVAL, leftIRCode);
TimedAction rightIRDetect = TimedAction(TASK_INTERVAL, rightIRCode);

TimedAction mazeMapping = TimedAction(TASK_INTERVAL, mazeMappingCode);
TimedAction getFront = TimedAction(TASK_INTERVAL, getFrontGrid);
TimedAction getLeft = TimedAction(TASK_INTERVAL, getLeftGrid);
TimedAction getRight = TimedAction(TASK_INTERVAL, getRightGrid);

volatile uint32_t leftPulse = 0;
volatile uint32_t rightPulse = 0;
int row = 11;
int col = 0;
int row_num = 0;
int col_num = 0;

bool No_Wall[12][12] = { false };
bool North_Wall[12][12] = { false };
bool South_Wall[12][12] = { false };
bool East_Wall[12][12] = { false };
bool West_Wall[12][12] = { false };

bool frontCellVisited[12][12] = { false };
bool leftCellVisited[12][12] = { false };
bool rightCellVisited[12][12] = { false };
uint16_t frontCellJunction[12][12] = { 0 };
uint16_t leftCellJunction[12][12] = { 0 };
uint16_t rightCellJunction[12][12] = { 0 };
// bool isEntry = false;
// bool isExit = true;

void initialiseWallMaze(){
  for(int i = 0; i < SIZE; i++){
    for(int j = 0; j < SIZE; j++){
      No_Wall[i][j] = false;
      North_Wall[i][j] = false;
      South_Wall[i][j] = false;
      East_Wall[i][j] = false;
      West_Wall[i][j] = false;

      frontCellVisited[i][j] = false;
      leftCellVisited[i][j] = false;
      rightCellVisited[i][j] = false;
      frontCellJunction[i][j] = 0;
      leftCellJunction[i][j] = 0;
      rightCellJunction[i][j] = 0;
    }
  }
}

bool isValid(int row, int col) {
  return (row >= 0 && col >= 0 && row < SIZE && col < SIZE);
}

void GetLeftPulseTaskCode() { cli(); sei(); }
void leftCounter() { leftPulse++; }

void GetRightPulseTaskCode() { cli(); sei(); }
void rightCounter() { rightPulse++; }

void frontDetectCode() { frontSensor.retrieveData(); }

void leftDetectCode() { leftSensor.retrieveData(); }

void rightDetectCode() { rightSensor.retrieveData(); }

void frontIRCode() { IRFront.retrieveData(); }

void leftIRCode() { IRLeft.retrieveData(); }

void rightIRCode() { IRRight.retrieveData(); }

//Update the maze based on encoder reading
void updateMazeCode() {
  if(isValid(row, col)) {
    switch(movement) {
    case FORWARD: {
      switch(direction) {
        case NORTH:
          { if(leftPulse >= PULSE_PER_GRID) { row_num = leftPulse / PULSE_PER_GRID; row-=row_num; leftPulse = 0; myRobot.visited[row][col] = true; break; } }
        case SOUTH:
          { if(leftPulse >= PULSE_PER_GRID) { row_num = leftPulse / PULSE_PER_GRID; row+=row_num; leftPulse = 0; myRobot.visited[row][col] = true; break; } }
        case WEST:
          { if(leftPulse >= PULSE_PER_GRID) { col_num = leftPulse / PULSE_PER_GRID; col-=col_num; leftPulse = 0; myRobot.visited[row][col] = true; break; } }
        case EAST:
          { if(leftPulse >= PULSE_PER_GRID) { col_num = leftPulse / PULSE_PER_GRID; col+=col_num; leftPulse = 0; myRobot.visited[row][col] = true; break; } }
      }
    }
    case LEFT:
    case RIGHT:
    case REVERSE:
      { leftPulse = 0; break; }
    }
  }
  Serial.print("Row: "); Serial.print(row); Serial.print("     "); Serial.print("Col: "); Serial.println(col); 
}

//Check for wall
void checkMovementCode() {
  // if(isValid(row, col)) {
  //   if(frontSensor.data < DIST_THRESH && rightSensor.data < DIST_THRESH && leftSensor.data < DIST_THRESH) { // dead end
  //     movement = REVERSE; 
  //     myRobot.Maze[row][col] = WALL; 
  //     switch(direction) {
  //       case NORTH: { myRobot.Maze[row+1][col] = 0; }
  //       case SOUTH: { myRobot.Maze[row-1][col] = 0; }
  //       case EAST: { myRobot.Maze[row][col-1] = 0; }
  //       case WEST: { myRobot.Maze[row][col+1] = 0; }
  //     }
  //   } 
  //   else if(frontSensor.data > DIST_THRESH && rightSensor.data < DIST_THRESH && leftSensor.data < DIST_THRESH) { movement = FORWARD; } // obstacle at left & right
  //   else if(frontSensor.data < DIST_THRESH && rightSensor.data < DIST_THRESH && leftSensor.data > DIST_THRESH) { movement = LEFT; } // obstacle at front & right
  //   else if(frontSensor.data < DIST_THRESH && rightSensor.data > DIST_THRESH && leftSensor.data < DIST_THRESH) { movement = RIGHT; } // obstacle at front & left
  //   else if(frontSensor.data < DIST_THRESH && rightSensor.data > DIST_THRESH && leftSensor.data > DIST_THRESH) { movement = LEFT; } // obstacle at front
  //   else if(frontSensor.data > DIST_THRESH && rightSensor.data > DIST_THRESH && leftSensor.data < DIST_THRESH) { movement = FORWARD; } // obstacle at left
  //   else if(frontSensor.data > DIST_THRESH && rightSensor.data < DIST_THRESH && leftSensor.data > DIST_THRESH) { movement = FORWARD; } // obstacle at right
  //   else { movement = FORWARD; } // no obstacle ahead 
  // }

  if(isValid(row, col)) {
    if(IRFront.status && IRLeft.status && IRRight.status) { // dead end
      movement = REVERSE; 
      pathType = DEAD_END;
      myRobot.Maze[row][col] = WALL; 
      switch(direction) {
        case NORTH: { myRobot.visited[row+1][col] = false; myRobot.Maze[row+1][col] = 0; myRobot.visited[row][col] = false; }
        case SOUTH: { myRobot.visited[row-1][col] = false; myRobot.Maze[row-1][col] = 0; myRobot.visited[row][col] = false; }
        case EAST: { myRobot.visited[row][col-1] = false; myRobot.Maze[row][col-1] = 0; myRobot.visited[row][col] = false; }
        case WEST: { myRobot.visited[row][col+1] = false; myRobot.Maze[row][col+1] = 0; myRobot.visited[row][col] = false; }
      }
    } 
    else if(!IRFront.status && IRLeft.status && IRRight.status) { movement = FORWARD; pathType = STRAIGHT_LEFT_RIGHT; } // obstacle at left & right
    else if(IRFront.status && !IRLeft.status && IRRight.status) { movement = LEFT; pathType = RIGHT_CORNER; } // obstacle at front & right
    else if(IRFront.status && IRLeft.status && !IRRight.status) { movement = RIGHT; pathType = LEFT_CORNER; } // obstacle at front & left
    else if(IRFront.status && !IRLeft.status && !IRRight.status) { movement = LEFT; pathType = T_JUNCTION; } // obstacle at front
    else if(!IRFront.status && IRLeft.status && !IRRight.status) { movement = FORWARD; pathType = STRAIGHT_LEFT; } // obstacle at left
    else if(!IRFront.status && !IRLeft.status && IRRight.status) { movement = FORWARD; pathType = STRAIGHT_RIGHT;} // obstacle at right
    else { movement = FORWARD; pathType = STRAIGHT_LEFT_RIGHT; } // no obstacle ahead 
  }
}

//Execute the locomotion code
void executeMovementCode() {
  Serial.print("Maze array element: "); Serial.print(myRobot.Maze[row][col]); Serial.print("    ");
  if(myRobot.Maze[row][col] != WALL && myRobot.Maze[row][col] != END && isValid(row, col)) {
    switch(movement) {
     case FORWARD:{
        leftWheelObj.moveForward();
        rightWheelObj.moveForward();
        Serial.println("Forward");
        break;
     }
      case LEFT: {
        checkMovement.disable();
        leftPulse = 0;
        // rightPulse = 0;
        while(leftPulse < 11) {
          leftWheelObj.moveLeft();
          rightWheelObj.moveLeft();
          Serial.println("Left"); 
        }
        leftPulse = 0;
        // rightPulse = 0;        
        if(direction == NORTH) { direction = WEST; }
        else if(direction == SOUTH) { direction = EAST; }
        else if(direction == WEST) { direction = SOUTH; }
        else { direction = NORTH; }
        checkMovement.enable();
        break;
      }

      case RIGHT: {
        checkMovement.disable();
        leftPulse = 0;
        // rightPulse = 0;
        while (leftPulse < 11) {
          leftWheelObj.moveRight();
          rightWheelObj.moveRight();
          Serial.println("Right");
        }
        leftPulse = 0;
        // rightPulse = 0;
        if(direction == NORTH) { direction = EAST; }
        else if(direction == SOUTH) { direction = WEST; }
        else if(direction == WEST) { direction = NORTH; }
        else { direction = SOUTH; }
        checkMovement.enable();
        break;
      }
      case REVERSE: {
        leftWheelObj.moveReverse();
        rightWheelObj.moveReverse();
        Serial.println("Reverse");
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
  } else {
    leftWheelObj.moveStop();
    rightWheelObj.moveStop();
    Serial.print("Stop"); Serial.print("    ");
    Serial.print(direction); Serial.print("    ");
  }
  // if(myRobot.Maze[row][col] == END) {
  //   leftWheelObj.moveStop();
  //   rightWheelObj.moveStop();
  //    Serial.println("Stop");
  // }
}

//Help map the maze
void mazeMappingCode() {
  myRobot.visited[row][col] = true;
  switch(pathType) {
    case STRAIGHT_LEFT_RIGHT:
      switch(direction) {
        case NORTH:
        case SOUTH:
          East_Wall[row][col] = true;
          West_Wall[row][col] = true;
          break;
        case EAST:
        case WEST:
          North_Wall[row][col] = true;
          South_Wall[row][col] = true;
          break;
        default: break;
      }
    case STRAIGHT_LEFT:
      switch(direction) {
        case NORTH:
          West_Wall[row][col] = true;
          break;
        case SOUTH:
          East_Wall[row][col] = true;
          break;
        case EAST:
          North_Wall[row][col] = true;
          break;
        case WEST:
          South_Wall[row][col] = true;
          break;
        default: break;
      }
    case STRAIGHT_RIGHT:
      switch(direction) {
        case NORTH:
          East_Wall[row][col] = true;
          break;
        case SOUTH:
          West_Wall[row][col] = true;
          break;
        case EAST:
          South_Wall[row][col] = true;
          break;
        case WEST:
          North_Wall[row][col] = true;
          break;
        default: break;
      }
    case RIGHT_CORNER:
      switch(direction) {
        case NORTH:
          East_Wall[row][col] = true;
          North_Wall[row][col] = true;
          break;
        case SOUTH:
          South_Wall[row][col] = true;
          West_Wall[row][col] = true;
          break;
        case EAST:
          East_Wall[row][col] = true;
          South_Wall[row][col] = true;
          break;
        case WEST:
          North_Wall[row][col] = true;
          West_Wall[row][col] = true;
          break;
        default: break;
      }
    case LEFT_CORNER:
      switch(direction) {
        case NORTH:
          North_Wall[row][col] = true;
          West_Wall[row][col] = true;
          break;
        case SOUTH:
          South_Wall[row][col] = true;
          East_Wall[row][col] = true;
          break;
        case EAST:
          North_Wall[row][col] = true;
          East_Wall[row][col] = true;
          break;
        case WEST:
          South_Wall[row][col] = true;
          West_Wall[row][col] = true;
          break;
        default: break;
    case T_JUNCTION:
      switch(direction) {
        case NORTH:
          North_Wall[row][col] = true;
          break;
        case SOUTH:
          South_Wall[row][col] = true;
          break;
        case EAST:
          East_Wall[row][col] = true;
          break;
        case WEST:
          West_Wall[row][col] = true;
          break;
        default: break;
      }
    case CROSS_JUNCTION:
      switch(direction) {
        case NORTH:
        case SOUTH:
        case EAST:
        case WEST:
          No_Wall[row][col] = true; 
          break; 
        default: break;
      }
      break;
    case DEAD_END:
      switch(direction) {
        case NORTH:
          North_Wall[row][col] = true;
          East_Wall[row][col] = true;
          West_Wall[row][col] = true;
          break;
        case SOUTH:
          South_Wall[row][col] = true;
          East_Wall[row][col] = true;
          West_Wall[row][col] = true;
          break;
        case EAST:
          South_Wall[row][col] = true;
          East_Wall[row][col] = true;
          North_Wall[row][col] = true;
          break;
        case WEST:
          South_Wall[row][col] = true;
          North_Wall[row][col] = true;
          West_Wall[row][col] = true;
          break;
        default: break;
      }
    }
  }
}

//Get the front grid coordinate
void getFrontGrid() {
  switch(direction) {
    case NORTH:
      frontCellVisited[row][col] = myRobot.Maze[row-1][col];
      frontCellJunction[row][col] = myRobot.visited[row-1][col];
      break;
    case SOUTH:
      frontCellVisited[row][col] = myRobot.Maze[row+1][col];
      frontCellJunction[row][col] = myRobot.visited[row+1][col];
      break;
    case EAST:
      frontCellVisited[row][col] = myRobot.Maze[row][col+1];
      frontCellJunction[row][col] = myRobot.visited[row][col+1];
      break;
    case WEST:
      frontCellVisited[row][col] = myRobot.Maze[row][col-1];
      frontCellJunction[row][col] = myRobot.visited[row][col-1];
      break;
    default: break;
  }
}

//Get the left grid coordinate
void getLeftGrid() {
  switch(direction) {
    case NORTH:
      leftCellVisited[row][col] = myRobot.Maze[row-1][col];
      leftCellJunction[row][col] = myRobot.visited[row-1][col];
      break;
    case SOUTH:
      leftCellVisited[row][col] = myRobot.Maze[row+1][col];
      leftCellJunction[row][col] = myRobot.visited[row+1][col];
      break;
    case EAST:
      leftCellVisited[row][col] = myRobot.Maze[row][col+1];
      leftCellJunction[row][col] = myRobot.visited[row][col+1];
      break;
    case WEST:
      leftCellVisited[row][col] = myRobot.Maze[row][col-1];
      leftCellJunction[row][col] = myRobot.visited[row][col-1];
      break;
    default: break;
  }
}

//Get the right grid coordinate
void getRightGrid() { 
  switch(direction) {
    case NORTH:
      rightCellVisited[row][col] = myRobot.Maze[row-1][col];
      rightCellJunction[row][col] = myRobot.visited[row-1][col];
      break;
    case SOUTH:
      rightCellVisited[row][col] = myRobot.Maze[row+1][col];
      rightCellJunction[row][col] = myRobot.visited[row+1][col];
      break;
    case EAST:
      rightCellVisited[row][col] = myRobot.Maze[row][col+1];
      rightCellJunction[row][col] = myRobot.visited[row][col+1];
      break;
    case WEST:
      rightCellVisited[row][col] = myRobot.Maze[row][col-1];
      rightCellJunction[row][col] = myRobot.visited[row][col-1];
      break;
    default: break;
  }
}

void setup() { 
  Serial.begin(9600);
  movement = FORWARD;
  direction = NORTH;
  // Serial.println(row);
  leftWheelObj.declarePin();
  rightWheelObj.declarePin();
  // frontSensor.declarePin();
  // rightSensor.declarePin();
  // leftSensor.declarePin();
  IRFront.declarePin();
  IRLeft.declarePin();
  IRRight.declarePin();
  pinMode(LEFT_ENC, INPUT);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC), leftCounter, RISING);
  pinMode(RIGHT_ENC, INPUT);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC), rightCounter, RISING);

  // frontDetect.enable();
  // leftDetect.enable();
  // rightDetect.enable();
  GetLeftPulseTask.enable();
  GetRightPulseTask.enable();
  checkMovement.enable();
  executeMovement.enable();
  updateMaze.enable();
  frontIRDetect.enable();
  leftIRDetect.enable();
  rightIRDetect.enable();
  mazeMapping.enable();
  getFront.enable();
  getLeft.enable();
  getRight.enable();
}

void loop() {
  // Serial.print("Left Encoder Pulse: "); Serial.print(leftPulse); Serial.print("    ");
  // Serial.print("Right Encoder Pulse:"); Serial.print(rightPulse); Serial.print("    ");
  // Serial.print("Front Sensor: "); Serial.print(frontSensor.data); Serial.print("    ");
  // Serial.print("Left Sensor: "); Serial.print(leftSensor.data); Serial.print("    ");
  // Serial.print("Right Sensor: "); Serial.print(rightSensor.data); Serial.println("    ");
  // Ser ft IR sensor: "); Serial.print(IRLeft.status); Serial.println("    ");
  // myRobot.testFunctions();
  initialiseWallMaze();
  myRobot.initialiseMaze();
  myRobot.IRsolveMaze();
  // frontDetect.check();
  // leftDetect.check();
  // rightDetect.check();
  frontIRDetect.check();
  leftIRDetect.check();
  rightIRDetect.check();
  GetLeftPulseTask.check();
  GetRightPulseTask.check();
  checkMovement.check();
  executeMovement.check();
  updateMaze.check();
  mazeMapping.check();
  getFront.check();
  getLeft.check();
  getRight.check();
}