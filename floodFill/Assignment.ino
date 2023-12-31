// Include necessary library & header files
#include <TimedAction.h>
#include "constant.h"
#include "ultrasonic.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "queue.h"
#include "maze.h"

// Create objects for parts of the robot
leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR_1, LEFT_DIR_2, 200);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR_1, RIGHT_DIR_2, 200);
ultrasonic frontSensor(FRONT_TRIG, FRONT_ECHO);
ultrasonic rightSensor(RIGHT_TRIG, RIGHT_ECHO);
ultrasonic leftSensor(LEFT_TRIG, LEFT_ECHO);
queueType rowQueue;
queueType colQueue;

maze robot(leftWheelObj, rightWheelObj, frontSensor, leftSensor, rightSensor);

int row = 11; // Initial point at row 11
int col = 0; // Initial point at col 0
int Maze[SIZE][SIZE] = { 0 }; // Initialize the array to 0
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

// Declare TimedAction functions
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

// Return the smaller value between 2 values
int minimum(int a, int b) {
  if(a < b) { return a; }
  return b;
}

// Print the Maze with floodfill values
void printMaze() {
  for(int i = 0; i < SIZE; i++) {
    for(int j = 0; j < SIZE; j++) {
      Serial.print(Maze[i][j]); Serial.print(" ");
    }
    Serial.println("");
  }
}

// Check if the coordinates is within the maze
bool isValid(int row, int col) {
  return(row >= 0 && col >= 0 && row < SIZE && col < SIZE);
}

// Reset the Wall State back everytime we enter a new cell
void initializeWallState() {
  NORTH_WALL = false;
  SOUTH_WALL = false;
  EAST_WALL = false;
  WEST_WALL = false;
}
// Check if there is a wall on the 4 sides of a cell
void checkWallCode() {
  // retrieve data values from ultrasonic sensors
  float frontData = frontSensor.retrieveData();
  float rightData = rightSensor.retrieveData();
  float leftData = leftSensor.retrieveData();
  Serial.print("Front: "); Serial.print(frontData); Serial.print("    ");
  Serial.print("Left: "); Serial.print(leftData); Serial.print("    ");
  Serial.print("Right: "); Serial.print(rightData); Serial.print("    ");
  // Compare front ultrasonic sensor data
  if(frontData < FRONT_DIST_THRESH) { 
    switch(direction) {
      case NORTH: horizontalWall[row][col]++; break;
      case SOUTH: horizontalWall[row][col] += 3; break;
      case EAST: verticalWall[row][col] += 2; break;
      case WEST: verticalWall[row][col] += 4; break;
    }
  }
  // Compare left ultrasonic sensor data
  if(leftData < SIDE_DIST_THRESH) { 
    switch(direction) {
      case NORTH: verticalWall[row][col] += 4; break;
      case SOUTH: verticalWall[row][col] += 2; break;
      case EAST: horizontalWall[row][col]++; break;
      case WEST: horizontalWall[row][col] += 3; break;
    }
  }
  // Compare right ultrasonic sensor data
  if(rightData < SIDE_DIST_THRESH) { 
    switch(direction) {
      case NORTH: verticalWall[row][col] += 2; break;
      case SOUTH: verticalWall[row][col] += 4; break;
      case EAST: horizontalWall[row][col] += 3; break;
      case WEST: horizontalWall[row][col]++; break;
    }
  }  
  Serial.println("CheckWallCode Done");
}

// Check if the adjacent cell is possible to move to
// If there is a wall, do not add the corresponding coordinates to the queue and update the flag(s)
// Else, add the corresponding coordinates to the queue and update the flag(s)
void checkAvailableCode() {
  // Check the available of the North & South coordinates
  switch(horizontalWall[row][col]) {
    case 0: rowQueue.enqueue(row-1); colQueue.enqueue(col); rowQueue.enqueue(row+1); colQueue.enqueue(col); break;
    case 1: rowQueue.enqueue(row+1); colQueue.enqueue(col); NORTH_WALL = true; break;
    case 3: rowQueue.enqueue(row-1); colQueue.enqueue(col); SOUTH_WALL = true; break;
    case 4: NORTH_WALL = true; SOUTH_WALL = true; break;
    default: break;
  }

  // Check the available of the East & West coordinates
  switch(verticalWall[row][col]) {
    case 0: rowQueue.enqueue(row); colQueue.enqueue(col-1); rowQueue.enqueue(row); colQueue.enqueue(col+1); break;
    case 2: rowQueue.enqueue(row); colQueue.enqueue(col-1); EAST_WALL = true; break;
    case 4: rowQueue.enqueue(row); colQueue.enqueue(col+1); WEST_WALL = true; break;
    case 6: EAST_WALL = true; WEST_WALL = true; break;
    default: break; 
  }
  Serial.println("CheckAvailableCode Done");
}

// Check the shortest manhattan distance and set it to the next move
int checkNextMoveCode() {
  int minDist = 150; // Initialize the manhattan distance to a value larger than 144
  if(isValid(row, col)) { // If the coordinates is within the maze, set minDist as the smallest value
    if(!NORTH_WALL && Maze[row][col] != 0) { minDist = min(minDist, Maze[row-1][col]); }
    if(!EAST_WALL && Maze[row][col] != 0) { minDist = min(minDist, Maze[row][col+1]); }
    if(!WEST_WALL && Maze[row][col] != 0) { minDist = min(minDist, Maze[row][col-1]); }
    if(!SOUTH_WALL && Maze[row][col] != 0) { minDist = min(minDist, Maze[row+1][col]); }
  }
  initializeWallState(); // Reset the wall states for the next cell
  Serial.print("minDist: "); Serial.println(minDist);
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
    // printMaze();
  }
  Serial.println("floodFillCode done");
}

void updateMazeCode() {
  switch(movement) {
    case FORWARD: {
      switch(direction) {
        case NORTH:
          { if(leftPulse % PULSE_PER_GRID == 0) { row--; break; } }
        case SOUTH:
          { if(leftPulse % PULSE_PER_GRID == 0) { row++; break; } }
        case WEST:
          { if(leftPulse % PULSE_PER_GRID == 0) { col--; break; } }
        case EAST:
          { if(leftPulse % PULSE_PER_GRID == 0) { col++; break; } }
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
        while(leftPulse < 11) {
          leftWheelObj.moveLeft();
          rightWheelObj.moveLeft();
        }
        leftPulse = 0; 
        if(direction == NORTH) { direction = WEST; }
        else if(direction == SOUTH) { direction = EAST; }
        else if(direction == WEST) { direction = SOUTH; }
        else { direction = NORTH; }
        checkWall.enable();
        break;
      case RIGHT:
        checkWall.disable();
        leftPulse = 0; 
        while(leftPulse < 11) {
          leftWheelObj.moveRight();
          rightWheelObj.moveRight();
        }
        leftPulse = 0; 
        if(direction == NORTH) { direction = EAST; }
        else if(direction == SOUTH) { direction = WEST; }
        else if(direction == WEST) { direction = NORTH; }
        else { direction = SOUTH; }
        checkWall.enable();
        break;
      case REVERSE: 
        checkWall.disable();
        leftPulse = 0; 
        while (leftPulse < 21) {
          leftWheelObj.moveRight();
          rightWheelObj.moveRight();
        }
        leftPulse = 0 ;
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
  Serial.println("executeMovementCode done")
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
  Serial.println("choosePathCode done");
}

void tunePositionCode() {
  long leftData = leftSensor.retrieveData();
  long rightData = rightSensor.retrieveData();
  if(leftData > 20 && leftData < 25) { leftWheelObj.speed = 220; }
  else { leftWheelObj.speed = 200; }
  if(rightData > 20 && rightData < 25) { rightWheelObj.speed = 220; }
  else { rightWheelObj.speed = 200; }
  Serial.println("tunePositionCode done");
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
  // getRightPulse.check();
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
  // getRightPulse.enable();
}

void setup() { 
  Serial.begin(115200);
  Maze[row][col] = 1;
  movement = FORWARD;
  direction = NORTH;
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

  // Serial.print("Front: "); Serial.print(frontSensor.retrieveData()); Serial.print("    ");
  // Serial.print("Left: "); Serial.print(leftSensor.retrieveData()); Serial.print("    ");
  // Serial.print("Right: "); Serial.print(rightSensor.retrieveData()); Serial.print("    ");
  Serial.print("Left Encoder Pulse: "); Serial.print(leftPulse); Serial.print("    ");
  // Serial.print("Right Encoder Pulse:"); Serial.println(rightPulse);

  checkTask();  
}