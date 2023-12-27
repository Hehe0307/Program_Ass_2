#include <TimedAction.h>

#include "constant.h"
#include "ultrasonic.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "queue.h"
#include "maze.h"

leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR_1, LEFT_DIR_2);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR_1, RIGHT_DIR_2);
ultrasonic frontSensor(FRONT_TRIG, FRONT_ECHO);
ultrasonic rightSensor(RIGHT_TRIG, RIGHT_ECHO);
ultrasonic leftSensor(LEFT_TRIG, LEFT_ECHO);
queueType rowQueue;
queueType colQueue;

maze robot(leftWheelObj, rightWheelObj, frontSensor, leftSensor, rightSensor);

int row = 11;
int col = 0;
int Maze[SIZE][SIZE] = { 0 };
int horizontalWall[SIZE][SIZE] = { 0 }; // 1 for North Wall, 3 for South Wall
int verticalWall[SIZE][SIZE] = { 0 }; // 4 for West Wall, 2 for East Wall

void leftCounter();
void rightCounter();
void getLeftPulseCode();
void getRightPulseCode();
int minimum(int, int);
bool isValid(int, int);
void initializeWallState();
void printMaze();
void checkTask();
void enableTask();
void updateMazeCode();
void executeMovementCode();
void choosePathCode();
int checkNextMoveCode();
void checkAvailableCode();
void checkWallCode();
void floodFillCode(int maze[SIZE][SIZE], int startRow, int startCol);
void tunePositionCode();

volatile uint32_t leftPulse = 0;
volatile uint32_t rightPulse = 0;

void leftCounter() { leftPulse++; }
void rightCounter() { rightPulse++; }
void getLeftPulseCode() { cli(); sei(); }
void getRightPulseCode() { cli(); sei(); }

TimedAction updateMaze = TimedAction(TASK_INTERVAL, updateMazeCode);
TimedAction executeMovement = TimedAction(TASK_INTERVAL, executeMovementCode);
TimedAction choosePath = TimedAction(TASK_INTERVAL, choosePathCode);
TimedAction checkWall = TimedAction(TASK_INTERVAL, checkWallCode);
TimedAction checkAvailable = TimedAction(TASK_INTERVAL, checkAvailableCode);
TimedAction checkNextMove = TimedAction(TASK_INTERVAL, checkNextMoveCode);
TimedAction floodFill = TimedAction(TASK_INTERVAL, floodFillCode);
TimedAction getLeftPulse = TimedAction(TASK_INTERVAL, getLeftPulseCode);
TimedAction getRightPulse = TimedAction(TASK_INTERVAL, getRightPulseCode);
TimedAction tunePosition = TimedAction(TASK_INTERVAL, tunePositionCode);

int minimum(int a, int b) {
  if(a < b) { return a; }
  return b;
}

void printMaze() {
  for(int i = 0; i < SIZE; i++) {
    for(int j = 0; j < SIZE; j++) {
      Serial.print(Maze[i][j]); Serial.print(" ");
    }
    Serial.println("");
  }
}

void checkWallCode() {
  float frontData = frontSensor.retrieveData();
  float rightData = rightSensor.retrieveData();
  float leftData = leftSensor.retrieveData();
  if(frontData < FRONT_DIST_THRESH) { 
    switch(direction) {
      case NORTH: horizontalWall[row][col]++; break;
      case SOUTH: horizontalWall[row][col] += 3; break;
      case EAST: verticalWall[row][col] += 2; break;
      case WEST: verticalWall[row][col] += 4; break;
    }
  }
  if(leftData < SIDE_DIST_THRESH) { 
    switch(direction) {
      case NORTH: verticalWall[row][col] += 4; break;
      case SOUTH: verticalWall[row][col] += 2; break;
      case EAST: horizontalWall[row][col]++; break;
      case WEST: horizontalWall[row][col] += 3; break;
    }
  }
  if(rightData < SIDE_DIST_THRESH) { 
    switch(direction) {
      case NORTH: verticalWall[row][col] += 2; break;
      case SOUTH: verticalWall[row][col] += 4; break;
      case EAST: horizontalWall[row][col] += 3; break;
      case WEST: horizontalWall[row][col]++; break;
    }
  }  
}

void checkAvailableCode() {
  switch(horizontalWall[row][col]) {
    case 0: rowQueue.enqueue(row-1); colQueue.enqueue(col); rowQueue.enqueue(row+1); colQueue.enqueue(col); break;
    case 1: rowQueue.enqueue(row+1); colQueue.enqueue(col); NORTH_WALL = true; break;
    case 3: rowQueue.enqueue(row-1); colQueue.enqueue(col); SOUTH_WALL = true; break;
    case 4: NORTH_WALL = true; SOUTH_WALL = true; break;
    default: break;
  }

  switch(verticalWall[row][col]) {
    case 0: rowQueue.enqueue(row); colQueue.enqueue(col-1); rowQueue.enqueue(row); colQueue.enqueue(col+1); break;
    case 2: rowQueue.enqueue(row); colQueue.enqueue(col-1); EAST_WALL = true; break;
    case 4: rowQueue.enqueue(row); colQueue.enqueue(col+1); WEST_WALL = true; break;
    case 6: EAST_WALL = true; WEST_WALL = true; break;
    default: break; 
  }
}

bool isValid(int row, int col) {
  return(row >= 0 && col >= 0 && row < SIZE && col < SIZE);
}

void initializeWallState() {
  NORTH_WALL = false;
  SOUTH_WALL = false;
  EAST_WALL = false;
  WEST_WALL = false;
}

int checkNextMoveCode() {
  int minDist = 150;
  if(isValid(row, col)) {
    if(!NORTH_WALL && Maze[row][col] != 0) { minDist = min(minDist, Maze[row-1][col]); }
    if(!EAST_WALL && Maze[row][col] != 0) { minDist = min(minDist, Maze[row][col+1]); }
    if(!WEST_WALL && Maze[row][col] != 0) { minDist = min(minDist, Maze[row][col-1]); }
    if(!SOUTH_WALL && Maze[row][col] != 0) { minDist = min(minDist, Maze[row+1][col]); }
  }
  initializeWallState();
  return minDist;
}

void floodFillCode(int maze[SIZE][SIZE], int startRow, int startCol) {
  int dr[] = { -1, 1, 0, 0 };
  int dc[] = { 0, 0, -1, 1 };

  for(int i = 0; i < SIZE; i++) {
    for(int j = 0; j < SIZE; j++) {
      if(maze[i][j] != 0) {
        maze[i][j] = -1; // Blank state
      }
    }
  }

  // Set startRow and startCol to 6
  maze[startRow][startCol] = 0;
  maze[startRow-1][startCol] = 0;
  maze[startRow][startCol-1] = 0;
  maze[startRow-1][startCol-1] = 0;

  rowQueue.enqueue(startRow);
  colQueue.enqueue(startCol);
  rowQueue.enqueue(startRow - 1);
  colQueue.enqueue(startCol);
  rowQueue.enqueue(startRow);
  colQueue.enqueue(startCol - 1);
  rowQueue.enqueue(startRow - 1);
  colQueue.enqueue(startCol - 1);

  while(!rowQueue.isEmptyQueue() && !colQueue.isEmptyQueue()) {
    //Dequeue the front position
    rowQueue.dequeue();
    colQueue.dequeue();

    int Row = rowQueue.queueFront;
    int Col = colQueue.queueFront;

    for(int i = 0; i < 4; i++){
      int newRow = Row + dr[i];
      int newCol = Col + dc[i];

      if(isValid(newRow, newCol) && maze[newRow][newCol] != -1){
        maze[newRow][newCol]++;

        rowQueue.enqueue(newRow);
        colQueue.enqueue(newCol);
      }
    }
    printMaze();
  }
}

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
  Serial.print("Row: "); Serial.print(row); Serial.print("     "); Serial.print("Col"); Serial.println(col); 
}

void executeMovementCode() {
  if(Maze[row][col] != 0) {
    switch(movement) {
      case FORWARD:
        leftWheelObj.moveForward();
        rightWheelObj.moveForward();
        break;
      case LEFT: 
        checkWall.disable();
        leftPulse = 0; 
        rightPulse = 0;
        while(leftPulse < 11 && rightPulse < 11) {
          leftWheelObj.moveLeft();
          rightWheelObj.moveLeft();
        }
        leftPulse = 0; 
        rightPulse = 0;
        if(direction == NORTH) { direction = WEST; }
        else if(direction == SOUTH) { direction = EAST; }
        else if(direction == WEST) { direction = SOUTH; }
        else { direction = NORTH; }
        checkWall.enable();
        break;
      case RIGHT:
        checkWall.disable();
        leftPulse = 0; 
        rightPulse = 0;
        while(leftPulse < 11 && rightPulse < 11) {
          leftWheelObj.moveRight();
          rightWheelObj.moveRight();
        }
        leftPulse = 0; 
        rightPulse = 0;
        if(direction == NORTH) { direction = EAST; }
        else if(direction == SOUTH) { direction = WEST; }
        else if(direction == WEST) { direction = NORTH; }
        else { direction = SOUTH; }
        checkWall.enable();
        break;
      case REVERSE: 
        checkWall.disable();
        leftPulse = 0; 
        rightPulse = 0;
        while (leftPulse < 21 && rightPulse < 21) {
          leftWheelObj.moveRight();
          rightWheelObj.moveRight();
        }
        leftPulse = 0 ;
        rightPulse = 0;
        if(direction == NORTH) { direction = SOUTH; }
        else if(direction == SOUTH) { direction = NORTH; }
        else if(direction == WEST) { direction = EAST; }
        else { direction = WEST; }
        checkWall.enable();
        break;
      default:
        break;
    }
  }
  if(Maze[row][col] == 0) {
    leftWheelObj.moveStop();
    rightWheelObj.moveStop();
  }
}

void choosePathCode() {
  int Dist = checkNextMoveCode;
  switch(direction) {
    case NORTH:
      if(Dist == Maze[row-1][col]) { movement = FORWARD; }
      if(Dist == Maze[row+1][col]) { movement = REVERSE; }
      if(Dist == Maze[row][col-1]) { movement = LEFT; }
      if(Dist == Maze[row][col+1]) { movement = RIGHT; }
      break;
    case SOUTH:
      if(Dist == Maze[row-1][col]) { movement = REVERSE; }
      if(Dist == Maze[row+1][col]) { movement = FORWARD; }
      if(Dist == Maze[row][col-1]) { movement = RIGHT; }
      if(Dist == Maze[row][col+1]) { movement = LEFT; }
      break;
    case EAST:
      if(Dist == Maze[row-1][col]) { movement = LEFT; }
      if(Dist == Maze[row+1][col]) { movement = RIGHT; }
      if(Dist == Maze[row][col-1]) { movement = REVERSE; }
      if(Dist == Maze[row][col+1]) { movement = FORWARD; }
      break;
    case WEST:
      if(Dist == Maze[row-1][col]) { movement = RIGHT; }
      if(Dist == Maze[row+1][col]) { movement = LEFT; }
      if(Dist == Maze[row][col-1]) { movement = FORWARD; }
      if(Dist == Maze[row][col+1]) { movement = REVERSE; }
      break;
    default: break;
  }
}

void tunePositionCode() {
  long leftData = leftSensor.retrieveData();
  long rightData = rightSensor.retrieveData();
  if(leftData > 20 && leftData < 25) { leftWheelObj.speed = 220; }
  else { leftWheelObj.speed = 200; }
  if(rightData > 20 && rightData < 25) { rightWheelObj.speed = 220; }
  else { rightWheelObj.speed = 200; }
}

void checkTask() {
  checkWall.check();
  checkAvailable.check();
  floodFill.check();
  checkNextMove.check();
  choosePath.check();
  executeMovement.check();
  updateMaze.check();
  tunePosition.check();
  getLeftPulse.check();
  getRightPulse.check();
}

void enableTask() {
  checkWall.enable();
  checkAvailable.enable();
  floodFill.enable();
  checkNextMove.enable();
  choosePath.enable();
  executeMovement.enable();
  updateMaze.enable();
  tunePosition.enable();
  getLeftPulse.enable();
  getRightPulse.enable();
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
  pinMode(RIGHT_ENC, INPUT);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC), rightCounter, RISING);

  enableTask();
}

void loop() {
  // robot.testFunctions();
  // robot.solveMaze();

  Serial.print("Left Encoder Pulse: "); Serial.print(leftPulse); Serial.print("    ");
  Serial.print("Right Encoder Pulse:"); Serial.println(rightPulse);

  checkTask();  
}