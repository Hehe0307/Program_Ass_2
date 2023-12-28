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

int row_num = 0;
int col_num = 0;
int row = 11; // Initial point at row 11
int col = 0; // Initial point at col 0
int Maze[SIZE][SIZE]; 
int horizontalWall[SIZE][SIZE] = { 0 }; // 1 for North Wall, 3 for South Wall
int verticalWall[SIZE][SIZE] = { 0 }; // 4 for West Wall, 2 for East Wall

void leftCounter();
void rightCounter();
void getLeftPulseCode();
void getRightPulseCode();

void initialiseMaze();
void updateMazeCode();
void executeMovementCode();
void tunePositionCode();
void frontDetectCode();
void leftDetectCode();
void rightDetectCode();

int minimum(int, int);
bool isValid(int, int);
void initializeWallState();
void printMaze();
void checkTask();
void enableTask();

void checkWallCode();
void checkAvailableCode();
void floodFillCode(int maze[SIZE][SIZE], int startRow, int startCol);
int checkNextMoveCode();
void choosePathCode();

volatile uint32_t leftPulse = 0;
volatile uint32_t rightPulse = 0;

void leftCounter() { leftPulse++; }
void rightCounter() { rightPulse++; }
void getLeftPulseCode() { cli(); sei(); }
void getRightPulseCode() { cli(); sei(); }

// Declare TimedAction functions
TimedAction updateMaze = TimedAction(1000, updateMazeCode);
TimedAction executeMovement = TimedAction(100, executeMovementCode);
TimedAction getLeftPulse = TimedAction(1, getLeftPulseCode);
TimedAction frontDetect = TimedAction(100, frontDetectCode);
TimedAction leftDetect = TimedAction(100, leftDetectCode);
TimedAction rightDetect = TimedAction(100, rightDetectCode);

// TimedAction tunePosition = TimedAction(TASK_INTERVAL, tunePositionCode);
// TimedAction checkWall = TimedAction(TASK_INTERVAL, checkWallCode);
// TimedAction checkAvailable = TimedAction(TASK_INTERVAL, checkAvailableCode);
// TimedAction floodFill = TimedAction(TASK_INTERVAL, floodFillCode);
// TimedAction checkNextMove = TimedAction(TASK_INTERVAL, checkNextMoveCode);
// TimedAction choosePath = TimedAction(TASK_INTERVAL, choosePathCode);
// TimedAction getRightPulse = TimedAction(TASK_INTERVAL, getRightPulseCode);

// Return the smaller value between 2 values
int minimum(int a, int b) {
  if(a < b) { return a; }
  return b;
}

// Initialize the array to -1
void initializeMaze(){
  for(int i = 0; i < 12; i++){
    for(int j = 0; j < 12; j++){
      Maze[i][j] = -1;
    }
  }
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
  // Compare front ultrasonic sensor data
  if((frontSensor.data < FRONT_DIST_THRESH) && isValid(row, col)) { 
    switch(direction) {
      case NORTH: horizontalWall[row][col]++; break; // North +1
      case SOUTH: horizontalWall[row][col] += 3; break; // South +3
      case EAST: verticalWall[row][col] += 2; break; // East +2
      case WEST: verticalWall[row][col] += 4; break; // West +4
    }
  }
  // Compare left ultrasonic sensor data
  if((leftSensor.data < SIDE_DIST_THRESH) && isValid(row, col)) { 
    switch(direction) {
      case NORTH: verticalWall[row][col] += 4; break;
      case SOUTH: verticalWall[row][col] += 2; break;
      case EAST: horizontalWall[row][col]++; break;
      case WEST: horizontalWall[row][col] += 3; break;
    }
  }
  // Compare right ultrasonic sensor data
  if((rightSensor.data < SIDE_DIST_THRESH) && isValid(row, col)) { 
    switch(direction) {
      case NORTH: verticalWall[row][col] += 2; break;
      case SOUTH: verticalWall[row][col] += 4; break;
      case EAST: horizontalWall[row][col] += 3; break;
      case WEST: horizontalWall[row][col]++; break;
    }
  }  
   Serial.print("CheckWall: ("); Serial.print(row); Serial.print(", "); Serial.print(col); Serial.println(")");
}

// Check if the adjacent cell is possible to move to
// If there is a wall, do not add the corresponding coordinates to the queue and update the flag(s)
// Else, add the corresponding coordinates to the queue and update the flag(s)
void checkAvailableCode() {
  if(isValid(row, col)) {
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
  }
   Serial.print("CheckAvailable: ("); Serial.print(row); Serial.print(", "); Serial.print(col); Serial.println(")");
}

void floodFillCode(int maze[SIZE][SIZE], int startRow, int startCol) {
  initializeMaze();

  rowQueue.initializeQueue();
  colQueue.initializeQueue();

  // rowQueue.dequeue();
  // colQueue.dequeue();

  int dr[4] = { -1, 1, 0, 0 };
  int dc[4] = { 0, 0, -1, 1 };

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
    int Row = rowQueue.dequeue();
    int Col = colQueue.dequeue();
  
    Serial.print("Row: "); Serial.print(Row); Serial.print("  Col: "); Serial.println(Col);
    
    //Dequeue the front position

    for(int i = 0; i < 4; i++){
      int newRow = Row + dr[i];
      int newCol = Col + dc[i];

      if(isValid(newRow, newCol) && maze[newRow][newCol] == -1){
        maze[newRow][newCol] = 0;
        maze[newRow][newCol]++;

        checkAvailableCode();
      }
    }
  printMaze();
  Serial.println();
  }
  Serial.println("floodFillCode done");
}

// Check the shortest manhattan distance and set it to the next move
int checkNextMoveCode() {
  int minDist = 150; // Initialize the manhattan distance to a value larger than 144
  if(isValid(row, col)) { // If the coordinates is within the maze, set minDist as the smallest value
    if(!NORTH_WALL && Maze[row][col] != 0) { minDist = minimum(minDist, Maze[row-1][col]); }
    if(!EAST_WALL && Maze[row][col] != 0) { minDist = minimum(minDist, Maze[row][col+1]); }
    if(!WEST_WALL && Maze[row][col] != 0) { minDist = minimum(minDist, Maze[row][col-1]); }
    if(!SOUTH_WALL && Maze[row][col] != 0) { minDist = minimum(minDist, Maze[row+1][col]); }
  }
  initializeWallState(); // Reset the wall states for the next cell
  Serial.print("NextMove: ("); Serial.print(row); Serial.print(", "); Serial.print(col); Serial.println(")");
  return minDist;
}

void choosePathCode() {
  int Dist = checkNextMoveCode();
  Serial.print("Distance: "); Serial.println(Dist);
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
    default:break;
  }
  Serial.print("ChoosePath: ("); Serial.print(row); Serial.print(", "); Serial.print(col); Serial.println(")");
}

void frontDetectCode() { frontSensor.retrieveData(); }

void leftDetectCode() { leftSensor.retrieveData(); }

void rightDetectCode() { rightSensor.retrieveData(); }

// No Issue
void updateMazeCode() {
  if(isValid(row, col)) {
    switch(movement) {
      case FORWARD: {
        switch(direction) {
          case NORTH:
            { if(leftPulse > PULSE_PER_GRID) { row_num = leftPulse / PULSE_PER_GRID; leftPulse = 0; row -= row_num; break; } }
          case SOUTH:
            { if(leftPulse > PULSE_PER_GRID) { row_num = leftPulse / PULSE_PER_GRID; leftPulse = 0; row += row_num; break; } }
          case WEST:
            { if(leftPulse > PULSE_PER_GRID) { col_num = leftPulse / PULSE_PER_GRID; leftPulse = 0; col-= col_num; break; } }
          case EAST:
            { if(leftPulse > PULSE_PER_GRID) { col_num = leftPulse / PULSE_PER_GRID; leftPulse = 0; col += col_num; break; } }
        }
      }
      case LEFT:
      case RIGHT:
      case REVERSE:
        { break; }
    }
  }
  Serial.print("Update: ("); Serial.print(row); Serial.print(", "); Serial.print(col); Serial.println(")");
}

void executeMovementCode() {
  if(isValid(row, col)) {
  Serial.print("Front: "); Serial.print(frontSensor.data); Serial.print("    ");
  Serial.print("Left: "); Serial.print(leftSensor.data); Serial.print("    ");
  Serial.print("Right: "); Serial.print(rightSensor.data); Serial.println("    ");
    switch(movement) {
      case FORWARD:
        leftWheelObj.moveForward();
        rightWheelObj.moveForward();
        Serial.println("Forward");
        break;
      case LEFT: 
        // checkWall.disable();
        // leftPulse = 0; 
        // while(leftPulse < 11) {
          leftWheelObj.moveLeft();
          rightWheelObj.moveLeft();
          Serial.println("Left");
        // }
        // leftPulse = 0; 
        if(direction == NORTH) { direction = WEST; }
        else if(direction == SOUTH) { direction = EAST; }
        else if(direction == WEST) { direction = SOUTH; }
        else { direction = NORTH; }
        // checkWall.enable();
        break;
      case RIGHT:
        // checkWall.disable();
        // leftPulse = 0; 
        // while(leftPulse < 11) {
          leftWheelObj.moveRight();
          rightWheelObj.moveRight();
          Serial.println("Right");
        // }
        // leftPulse = 0; 
        if(direction == NORTH) { direction = EAST; }
        else if(direction == SOUTH) { direction = WEST; }
        else if(direction == WEST) { direction = NORTH; }
        else { direction = SOUTH; }
        // checkWall.enable();
        break;
      case REVERSE: 
        // checkWall.disable();
        // leftPulse = 0; 
        // while (leftPulse < 21) {
          leftWheelObj.moveReverse();
          rightWheelObj.moveReverse();
          Serial.println("Reverse");
        // }
        // leftPulse = 0 ;
        if(direction == NORTH) { direction = SOUTH; }
        else if(direction == SOUTH) { direction = NORTH; }
        else if(direction == WEST) { direction = EAST; }
        else { direction = WEST; }
        // checkWall.enable();
        break;
      default:
        break;
    }
  } else {
    leftWheelObj.moveStop();
    rightWheelObj.moveStop();
    Serial.println("Stop");
  }
  // if(Maze[row][col] == 0) {
  //   leftWheelObj.moveStop();
  //   rightWheelObj.moveStop();
  // }
  checkWallCode();
  floodFillCode(Maze, 6, 6);
  choosePathCode();
  Serial.print("Execute: ("); Serial.print(row); Serial.print(", "); Serial.print(col); Serial.println(")");
  Serial.print(leftPulse);
}

void tunePositionCode() {
  if(leftSensor.data > 20 && leftSensor.data < 25) { leftWheelObj.speed = 150; }
  else { leftWheelObj.speed = 200; }
  if(rightSensor.data > 20 && rightSensor.data < 25) { rightWheelObj.speed = 150; }
  else { rightWheelObj.speed = 200; }
  Serial.println("tunePositionCode done");
}

void enableTask() {
  //  checkWall.enable(); //!!
  //  checkAvailable.enable(); //!!
  // floodFill.enable();
  // checkNextMove.enable();
  // choosePath.enable();
  executeMovement.enable(); //!!
  updateMaze.enable();
  // tunePosition.enable();
  getLeftPulse.enable();
  // getRightPulse.enable();

  frontDetect.enable();
  leftDetect.enable();
  rightDetect.enable();
}

void checkTask() {
  // checkWall.check(); //!!
  // checkAvailable.check(); //!!
  // floodFill.check();
  // checkNextMove.check();
  // choosePath.check();
  executeMovement.check(); //!!
  updateMaze.check();
  //tunePosition.check();
  getLeftPulse.check();
  //getRightPulse.check();

  frontDetect.check();
  leftDetect.check();
  rightDetect.check();
}

void setup() { 
  Serial.begin(115200);
  // movement = FORWARD;
  // direction = NORTH;
  //rowQueue.initializeQueue();
  //colQueue.initializeQueue();
  floodFillCode(Maze, 6, 6);
  leftWheelObj.declarePin();
  rightWheelObj.declarePin();
  frontSensor.declarePin();
  rightSensor.declarePin();
  leftSensor.declarePin();

  pinMode(LEFT_ENC, INPUT);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC), leftCounter, RISING);
  // pinMode(RIGHT_ENC, INPUT);
  // attachInterrupt(digitalPinToInterrupt(RIGHT_ENC), rightCounter, RISING);

  enableTask();
}

void loop() {
  // robot.testFunctions();
  // robot.solveMaze();

  // Serial.print("Left Encoder Pulse: "); Serial.print(leftPulse); Serial.println("    ");
  // Serial.print("Right Encoder Pulse:"); Serial.println(rightPulse);

  checkTask();  
}