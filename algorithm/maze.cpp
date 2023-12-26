#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "ultrasonic.h"
#include "encoder.h"
#include "maze.h"
#include "constant.h"
#include "queue.h"

#include "Arduino.h"

maze::maze(leftWheel LeftWheel, rightWheel RightWheel, ultrasonic Front, ultrasonic Left, ultrasonic Right, encoder LeftEnc, encoder rightEnc, Queue q1, Queue q2)
: LeftWheel(LeftWheel), RightWheel(RightWheel), Front(Front), Left(Left), Right(Right), LeftEnc(LeftEnc), RightEnc(RightEnc), q1(q1), q2(q2) {}

int maze::min(int a, int b) {
  if(a < b) { return a; }
  return b;
}

void maze::printMaze() {
  for(int i = 0; i < SIZE; i++) {
    for(int j = 0; j < SIZE; j++) {
      Serial.print(Maze[i][j]); Serial.print(" ");
    }
    Serial.println("");
  }
}

void maze::checkWallCode() {
  long frontData = Front.retrieveData();
  long leftData = Left.retrieveData();
  long rightData = Right.retrieveData();
  if(frontData < DIST_THRESH) { 
    switch(direction) {
      case NORTH: horizontalWall[row][col]++; break;
      case SOUTH: horizontalWall[row][col] += 3; break;
      case EAST: verticalWall[row][col] += 2; break;
      case WEST: verticalWall[row][col] += 4; break;
    }
  }
  if(leftData < DIST_THRESH) { 
    switch(direction) {
      case NORTH: verticalWall[row][col] += 4; break;
      case SOUTH: verticalWall[row][col] += 2; break;
      case EAST: horizontalWall[row][col]++; break;
      case WEST: horizontalWall[row][col] += 3; break;
    }
  }
  if(rightData < DIST_THRESH) { 
    switch(direction) {
      case NORTH: verticalWall[row][col] += 2; break;
      case SOUTH: verticalWall[row][col] += 4; break;
      case EAST: horizontalWall[row][col] += 3; break;
      case WEST: horizontalWall[row][col]++; break;
    }
  }  
}

void maze::checkAvailableCode() {
  switch(horizontalWall[row][col]) {
    case 0: q1.enqueue(row-1); q2.enqueue(col); q1.enqueue(row+1); q2.enqueue(col); break;
    case 1: q1.enqueue(row+1); q2.enqueue(col); NORTH_WALL = true; break;
    case 3: q1.enqueue(row-1); q2.enqueue(col); SOUTH_WALL = true; break;
    case 4: NORTH_WALL = true; SOUTH_WALL = true; break;
    default: break;
  }

  switch(verticalWall[row][col]) {
    case 0: q1.enqueue(row); q2.enqueue(col-1); q1.enqueue(row); q2.enqueue(col+1); break;
    case 2: q1.enqueue(row); q2.enqueue(col-1); EAST_WALL = true; break;
    case 4: q1.enqueue(row); q2.enqueue(col+1); WEST_WALL = true; break;
    case 6: EAST_WALL = true; WEST_WALL = true; break;
    default: break; 
  }
}

bool maze::isValid(int row, int col) {
  return(row >= 0 && col >= 0 && row < SIZE && col < SIZE);
}

void maze::initializeWallState() {
  NORTH_WALL = false;
  SOUTH_WALL = false;
  EAST_WALL = false;
  WEST_WALL = false;
}

int maze::checkNextMoveCode() {
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

void maze::floodFillCode(int maze[SIZE][SIZE], int startRow, int startCol, Queue q1, Queue q2) {
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

  q1.enqueue(startRow);
  q2.enqueue(startCol);
  q1.enqueue(startRow - 1);
  q2.enqueue(startCol);
  q1.enqueue(startRow);
  q2.enqueue(startCol - 1);
  q1.enqueue(startRow - 1);
  q2.enqueue(startCol - 1);

  while(!q.isEmptyQueue()) {
    //Dequeue the front position
    q1.dequeue();
    q2.dequeue();

    int Row = q1.dequeue();
    int Col = q2.dequeue();

    for(int i = 0; i < 4; i++){
      int newRow = Row + dr[i];
      int newCol = Col + dc[i];

      if(isValid(newRow, newCol) && maze[newRow][newCol] != -1){
        maze[newRow][newCol]++;

        q1.enqueue(newRow);
        q2.enqueue(newCol);
      }
    }
    printMaze();
  }
}

void maze::updateMazeCode() {
  switch(movement) {
    case FORWARD: {
      switch(direction) {
        case NORTH:
          { if(LeftEnc.pulse == RightEnc.pulse && LeftEnc.pulse % PULSE_PER_GRID == 0) { row--; break; } }
        case SOUTH:
          { if(LeftEnc.pulse == RightEnc.pulse && LeftEnc.pulse % PULSE_PER_GRID == 0) { row++; break; } }
        case WEST:
          { if(LeftEnc.pulse == RightEnc.pulse && LeftEnc.pulse % PULSE_PER_GRID == 0) { col--; break; } }
        case EAST:
          { if(LeftEnc.pulse == RightEnc.pulse && LeftEnc.pulse % PULSE_PER_GRID == 0) { col++; break; } }
      }
    }
    case LEFT:
    case RIGHT:
    case REVERSE:
      { break; }
  }
  Serial.print("Row: "); Serial.print(row); Serial.print("     "); Serial.print("Col"); Serial.println(col); 
}

void maze::executeMovementCode() {
  if(Maze[row][col] != 0) {
    switch(movement) {
      case FORWARD:
        leftWheelObj.moveForward();
        rightWheelObj.moveForward();
        break;
      case LEFT: 
        checkWall.disable();
        LeftEnc.pulse = 0;
        RightEnc.pulse = 0
        while(LeftEnc.pulse < 11 && RightEnc.pulse < 11) {
          leftWheelObj.moveLeft();
          rightWheelObj.moveLeft();
        }
        LeftEnc.pulse = 0;
        RightEnc.pulse = 0;
        if(direction == NORTH) { direction = WEST; }
        else if(direction == SOUTH) { direction = EAST; }
        else if(direction == WEST) { direction = SOUTH; }
        else { direction = NORTH; }
        checkWall.enable();
        break;
      case RIGHT:
        checkWall.disable();
        LeftEnc.pulse = 0;
        RightEnc.pulse = 0; 
        while(LeftEnc.pulse < 11 && RightEnc.pulse < 11) {
          leftWheelObj.moveRight();
          rightWheelObj.moveRight();
        }
        LeftEnc.pulse = 0;
        RightEnc.pulse = 0; 
        if(direction == NORTH) { direction = EAST; }
        else if(direction == SOUTH) { direction = WEST; }
        else if(direction == WEST) { direction = NORTH; }
        else { direction = SOUTH; }
        checkWall.enable();
        break;
      case REVERSE: 
        checkWall.disable();
        LeftEnc.pulse = 0;
        RightEnc.pulse = 0; 
        while (LeftEnc.pulse < 21 && RightEnc.pulse < 21) {
          leftWheelObj.moveRight();
          rightWheelObj.moveRight();
        }
        LeftEnc.pulse = 0;
        RightEnc.pulse = 0; 
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

void maze::choosePathCode() {
  int Dist = checkNextMoveCode();
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

void maze::solveMaze() {
  float frontData = Front.retrieveData();
  float rightData = Right.retrieveData();
  float leftData = Left.retrieveData();
  while(true) {
    if (frontData > DIST_THRESH && rightData < DIST_THRESH && leftData < DIST_THRESH) { // left & right have obstacles but front is clear
      LeftWheel.moveForward();
      RightWheel.moveForward(); 
      // Serial.println("Forward");
      // delay(100);
    }
    else if ( frontData < DIST_THRESH && rightData < DIST_THRESH && leftData < DIST_THRESH) { // obstacle infront of all 3 sides, reverse
      LeftWheel.moveReverse();
      RightWheel.moveReverse();
      // Serial.println("Reverse");
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
    else if (frontData < DIST_THRESH && rightData < DIST_THRESH && leftData > DIST_THRESH) { // obstacle on right and front sides, turn left side 
      LeftWheel.moveLeft(); 
      RightWheel.moveLeft(); 
      // Serial.println("Left");
      // delay(100);
    }
    else if (frontData < DIST_THRESH && rightData > DIST_THRESH && leftData < DIST_THRESH) { // obstacle on left and front sides, turn right side
      LeftWheel.moveRight();
      RightWheel.moveRight();
      // Serial.println("Right");
      // delay(100);
    }
    else  if (frontData < DIST_THRESH && rightData > DIST_THRESH && leftData > DIST_THRESH) { // obstacle on front side, turn left
      LeftWheel.moveLeft();
      LeftWheel.moveLeft(); 
    }
    else if (frontData > DIST_THRESH && rightData > DIST_THRESH && leftData < DIST_THRESH) { // obstacle on left side, forward
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Forward");
      // delay(100);
    }
    else if (frontData > DIST_THRESH && rightData < DIST_THRESH && leftData > DIST_THRESH) { // obstacle on right side, turn left & then forward
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Forward");
      // delay(100);
    }
    else { // no osbtacle ahead
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Forward");
      // delay(100);
    }
  }
}

void maze::pidLeftWheelCode() {

}

void maze::pidRightWheelCode() {

}

void maze::checkTask() {
  checkWall.check();
  checkAvailable.check();
  floodFill.check();
  checkNextMove.check();
  choosePath.check();
  executeMovement.check();
  pidLeftWheel.check();
  pidRightWheel.check();
  updateMaze.check();
}

void maze::enableTask() {
  checkWall.enable();
  checkAvailable.enable();
  floodFill.enable();
  checkNextMove.enable();
  choosePath.enable();
  executeMovement.enable();
  pidLeftWheel.enable();
  pidRightWheel.enable();
  updateMaze.enable();
}
