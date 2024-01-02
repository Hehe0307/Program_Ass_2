// Group 10
// Members: Eng Chee Wen 22050762/1  Lee Yi Jia 22004570/1  Lai Zhe Jun S2182269/1  Chong Wei Jie 22054152/1

// Include neccessary libraries & header files
#include <TimedAction.h>
#include "constant.h"
#include "ultrasonic.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "IR.h"
#include "robot.h"

// Object Declaration
leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR_1, LEFT_DIR_2, 130);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR_1, RIGHT_DIR_2, 130);
ultrasonic frontSensor(FRONT_TRIG, FRONT_ECHO);
ultrasonic rightSensor(RIGHT_TRIG, RIGHT_ECHO);
ultrasonic leftSensor(LEFT_TRIG, LEFT_ECHO);
IR IRFront(FRONT_IR);
IR IRLeft(LEFT_IR);
IR IRRight(RIGHT_IR);
robot myRobot(frontSensor, rightSensor, leftSensor, leftWheelObj, rightWheelObj, IRFront, IRLeft, IRRight);

// Function Prototypes
void initializeWallMaze();
void GetLeftPulseTaskCode();
void GetRightPulseTaskCode();
void checkMovementCode();
void executeMovementCode();
void updateMazeCode();
void frontIRCode();
void leftIRCode();
void rightIRCode();
void frontDetectCode();
void leftDetectCode();
void rightDetectCode();
void mazeMappingCode();
void getFrontGrid();
void getLeftGrid();
void getRightGrid();
void TuningTaskCode();
void PIDTaskCode();
void ForwardTaskCode();
void leftAverageValCode();
void rightAverageValCode();

// TimedAction Functions Declaration
TimedAction GetLeftPulseTask = TimedAction(1, GetLeftPulseTaskCode);
TimedAction GetRightPulseTask = TimedAction(TASK_INTERVAL, GetRightPulseTaskCode);
TimedAction checkMovement = TimedAction(TASK_INTERVAL, checkMovementCode);
TimedAction executeMovement = TimedAction(TASK_INTERVAL, executeMovementCode);
TimedAction updateMaze = TimedAction(TASK_INTERVAL, updateMazeCode);
TimedAction frontIRDetect = TimedAction(TASK_INTERVAL, frontIRCode);
TimedAction leftIRDetect = TimedAction(TASK_INTERVAL, leftIRCode);
TimedAction rightIRDetect = TimedAction(TASK_INTERVAL, rightIRCode);
TimedAction frontDetect = TimedAction(PID_TASK_INTERVAL, frontDetectCode);
TimedAction leftDetect = TimedAction(PID_TASK_INTERVAL, leftDetectCode);
TimedAction rightDetect = TimedAction(PID_TASK_INTERVAL, rightDetectCode);
TimedAction mazeMapping = TimedAction(TASK_INTERVAL, mazeMappingCode);
TimedAction getFront = TimedAction(TASK_INTERVAL, getFrontGrid);
TimedAction getLeft = TimedAction(TASK_INTERVAL, getLeftGrid);
TimedAction getRight = TimedAction(TASK_INTERVAL, getRightGrid);
TimedAction TuningTask = TimedAction(PID_TASK_INTERVAL, TuningTaskCode);
TimedAction PIDTask = TimedAction(TASK_INTERVAL, PIDTaskCode);
TimedAction ForwardTask = TimedAction(TASK_INTERVAL, ForwardTaskCode);
TimedAction leftAverageVal = TimedAction(PID_TASK_INTERVAL, leftAverageValCode);
TimedAction rightAverageVal = TimedAction(PID_TASK_INTERVAL, rightAverageValCode);

volatile uint32_t leftPulse = 0;
volatile uint32_t rightPulse = 0;
int row = 11;
int col = 0;
int row_num = 0;
int col_num = 0;

bool leftUsable = false;
bool rightUsable = false;
int leftCount = 0;
int rightCount = 0;
float leftAverage = 0.0;
float rightAverage = 0.0;
int iLeft = 0;
int iRight = 0;
long arrLeft[PID_SIZE];
long arrRight[PID_SIZE];

int error = 0;
int prevError = 0;
int derivative = 0;
int integral = 0;
int integralLimit = 500;
int PIDOutputVal = 0;
int PIDOutputMax = 40;
int kp = 30;
int ki = 0;
int kd = 0;

bool No_Wall[SIZE][SIZE];
bool North_Wall[SIZE][SIZE];
bool South_Wall[SIZE][SIZE];
bool East_Wall[SIZE][SIZE];
bool West_Wall[SIZE][SIZE];
bool frontCellVisited[SIZE][SIZE];
bool leftCellVisited[SIZE][SIZE];
bool rightCellVisited[SIZE][SIZE];
uint16_t frontCellJunction[SIZE][SIZE];
uint16_t leftCellJunction[SIZE][SIZE];
uint16_t rightCellJunction[SIZE][SIZE];
const int refMaze[SIZE][SIZE] = {
  { 12, 11, 10, 9, 8, 7, 6, 7, 8, 9, 10, 11, 12 },
  { 11, 10, 9, 8, 7, 6, 5, 6, 7, 8, 9, 10, 11 },
  { 10, 9, 8, 7, 6, 5, 4, 5, 6, 7, 8, 9, 10 },
  { 9, 8, 7, 6, 5, 4, 3, 4, 5, 6, 7, 8, 9 },
  { 8, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 8 },
  { 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7 },
  { 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6 },
  { 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7 },
  { 8, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 8 },
  { 9, 8, 7, 6, 5, 4, 3, 4, 5, 6, 7, 8, 9 },
  { 10, 9, 8, 7, 6, 5, 4, 5, 6, 7, 8, 9, 10 },
  { 11, 10, 9, 8, 7, 6, 5, 6, 7, 8, 9, 10, 11 },
  { 12, 11, 10, 9, 8, 7, 6, 7, 8, 9, 10, 11, 12 }
};

void initializeWallMaze() {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
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

void leftAverageValCode() {
  if(leftCount < PID_SIZE) {
    iLeft = leftCount & PID_SIZE;
    arrLeft[iLeft] = leftSensor.data;
    leftCount++;
    long total = 0;
    for(int val = 0; val < leftCount; val++) {
      total += arrLeft[val];
      Serial.print(arrLeft[val]); Serial.print("    ");
    }
    leftAverage = total * 1.0 / leftCount;
    Serial.println("");
    Serial.println(leftAverage);
  } else {
    leftUsable = true;
    iLeft = leftCount % PID_SIZE;
    arrLeft[iLeft] = leftSensor.data;
    leftCount++;
    long total = 0;
    for(int val = 0; val < leftCount; val++) {
      total += arrLeft[val];
      Serial.print(arrLeft[val]); Serial.print("    ");
    }
    leftAverage = total * 1.0 / leftCount;
    Serial.println("");
    Serial.println(leftAverage);
  }
}

void rightAverageValCode() {
  if(rightCount < PID_SIZE) {
    iRight = rightCount & PID_SIZE;
    arrRight[iRight] = rightSensor.data;
    rightCount++;
    long total = 0;
    for(int val = 0; val < rightCount; val++) {
      total += arrRight[val];
      Serial.print(arrRight[val]); Serial.print("    ");
    }
    rightAverage = total * 1.0 / rightCount;
    Serial.println("");
    Serial.println(rightAverage);
  } else {
    rightUsable = true;
    iRight = rightCount % PID_SIZE;
    arrRight[iRight] = rightSensor.data;
    rightCount++;
    long total = 0;
    for(int val = 0; val < rightCount; val++) {
      total += arrRight[val];
      Serial.print(arrRight[val]); Serial.print("    ");
    }
    rightAverage = total * 1.0 / rightCount;
    Serial.println("");
    Serial.println(rightAverage);
  }
}

void TuningTaskCode() {
  if(leftSensor.data < rightSensor.data) { rightWheelObj.speed--; }
  else if(leftSensor.data > rightSensor.data) { leftWheelObj.speed--; }
  else { leftWheelObj.speed = 130; rightWheelObj.speed = 130; }  
}

void ForwardTaskCode() {
  leftWheelObj.moveForward();
  rightWheelObj.moveForward();
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

void PIDTaskCode() {
  if (leftAverage >= SIDE_DIST_THRESH) {
    error = SIDE_DIST_THRESH - rightAverage;
  }

  else if (rightAverage >= SIDE_DIST_THRESH) {
    error = leftAverage - SIDE_DIST_THRESH;
  }

  else if (leftAverage >= SIDE_DIST_THRESH && rightAverage >= SIDE_DIST_THRESH) {
    return;
  }

  else {
    error = leftAverage - rightAverage;
  }

  if (error >= -5 && error <= 5) {
    PIDOutputVal = 0;
  }

  else {
    derivative = (error - prevError);
    integral += (5 * (error + prevError));

    if (integral > integralLimit) {
      integral = integralLimit;
    }

    else if (integral < -(integralLimit)) {
      integral = -(integralLimit);
    }

    PIDOutputVal = (kp * error + ki * integral + kd * derivative) / 1000;
  }

  if (PIDOutputVal > PIDOutputMax) {
    PIDOutputVal = PIDOutputMax;
  }

  else if (PIDOutputVal > -(PIDOutputMax)) {
    PIDOutputVal = -(PIDOutputMax);
  }

  cli();
  leftWheelObj.speed = leftWheelObj.speed - PIDOutputVal;
  rightWheelObj.speed = rightWheelObj.speed - PIDOutputVal;
  sei();

  prevError = error;

  Serial.println(error);
}

// Update the maze based on encoder reading
void updateMazeCode() {
  if (isValid(row, col)) {
    switch (movement) {
      case FORWARD:
        {
          switch (direction) {
            case NORTH:
              {
                if (leftPulse >= PULSE_PER_GRID) {
                  row_num = leftPulse / PULSE_PER_GRID;
                  row -= row_num;
                  leftPulse = 0;
                  myRobot.visited[row][col] = true;
                  break;
                }
              }
            case SOUTH:
              {
                if (leftPulse >= PULSE_PER_GRID) {
                  row_num = leftPulse / PULSE_PER_GRID;
                  row += row_num;
                  leftPulse = 0;
                  myRobot.visited[row][col] = true;
                  break;
                }
              }
            case WEST:
              {
                if (leftPulse >= PULSE_PER_GRID) {
                  col_num = leftPulse / PULSE_PER_GRID;
                  col -= col_num;
                  leftPulse = 0;
                  myRobot.visited[row][col] = true;
                  break;
                }
              }
            case EAST:
              {
                if (leftPulse >= PULSE_PER_GRID) {
                  col_num = leftPulse / PULSE_PER_GRID;
                  col += col_num;
                  leftPulse = 0;
                  myRobot.visited[row][col] = true;
                  break;
                }
              }
          }
        }
      case LEFT:
      case RIGHT:
        {
          leftPulse = 0;
          break;
        }
      case BACKWARD:
        {
          switch (direction) {
            case NORTH:
              {
                if (leftPulse >= PULSE_PER_GRID) {
                  row_num = leftPulse / PULSE_PER_GRID;
                  row += row_num;
                  leftPulse = 0;
                  myRobot.visited[row][col] = true;
                  break;
                }
              }
            case SOUTH:
              {
                if (leftPulse >= PULSE_PER_GRID) {
                  row_num = leftPulse / PULSE_PER_GRID;
                  row -= row_num;
                  leftPulse = 0;
                  myRobot.visited[row][col] = true;
                  break;
                }
              }
            case WEST:
              {
                if (leftPulse >= PULSE_PER_GRID) {
                  col_num = leftPulse / PULSE_PER_GRID;
                  col += col_num;
                  leftPulse = 0;
                  myRobot.visited[row][col] = true;
                  break;
                }
              }
            case EAST:
              {
                if (leftPulse >= PULSE_PER_GRID) {
                  col_num = leftPulse / PULSE_PER_GRID;
                  col -= col_num;
                  leftPulse = 0;
                  myRobot.visited[row][col] = true;
                  break;
                }
              }
          }
        }
    }
  }
  Serial.print("Row: ");
  Serial.print(row);
  Serial.print("     ");
  Serial.print("Col: ");
  Serial.println(col);
}

// Check for wall
void checkMovementCode() {
  // // For Ultrasonic Sensor
  // if(isValid(row, col)) {
  //   if(frontSensor.data < DIST_THRESH && rightSensor.data < DIST_THRESH && leftSensor.data < DIST_THRESH) { // dead end
  //     movement = BACKWARD;
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

  // For IR Sensor
  // Serial.print("Front IR sensor: ");
  // Serial.print(IRFront.status);
  // Serial.print("    ");
  // Serial.print("Right IR sensor: ");
  // Serial.print(IRRight.status);
  // Serial.print("    ");
  // Serial.print("Left IR sensor: ");
  // Serial.println(IRLeft.status);
  if (isValid(row, col)) {
    if (IRFront.status && IRLeft.status && IRRight.status) {  // dead end
      movement = BACKWARD;
      pathType = DEAD_END;
      myRobot.Maze[row][col] = WALL;
      switch (direction) {
        case NORTH:
          {
            myRobot.visited[row + 1][col] = false;
            myRobot.Maze[row + 1][col] = 0;
            myRobot.visited[row][col] = true;
          }
        case SOUTH:
          {
            myRobot.visited[row - 1][col] = false;
            myRobot.Maze[row - 1][col] = 0;
            myRobot.visited[row][col] = true;
          }
        case EAST:
          {
            myRobot.visited[row][col - 1] = false;
            myRobot.Maze[row][col - 1] = 0;
            myRobot.visited[row][col] = true;
          }
        case WEST:
          {
            myRobot.visited[row][col + 1] = false;
            myRobot.Maze[row][col + 1] = 0;
            myRobot.visited[row][col] = true;
          }
      }
    } else if (!IRFront.status && IRLeft.status && IRRight.status) {  // obstacle at left & right
      pathType = STRAIGHT_LEFT_RIGHT;
      movement = FORWARD;
    } else if (IRFront.status && !IRLeft.status && IRRight.status) {  // obstacle at front & right
      pathType = RIGHT_CORNER;
      movement = LEFT;
    } else if (IRFront.status && IRLeft.status && !IRRight.status) {  // obstacle at front & left
      pathType = LEFT_CORNER;
      movement = RIGHT;
    } else if (IRFront.status && !IRLeft.status && !IRRight.status) {  // obstacle at front
      pathType = T_JUNCTION;
      switch (direction) {
        case NORTH:
          if (refMaze[row][col - 1] > refMaze[row][col + 1]) { movement = RIGHT; }
          if (refMaze[row][col - 1] < refMaze[row][col + 1]) { movement = LEFT; }
          if (refMaze[row][col - 1] == refMaze[row][col + 1]) { movement = LEFT; }  // if equal, prioritize turn left
          break;
        case SOUTH:
          if (refMaze[row][col - 1] > refMaze[row][col + 1]) { movement = LEFT; }
          if (refMaze[row][col - 1] < refMaze[row][col + 1]) { movement = RIGHT; }
          if (refMaze[row][col - 1] == refMaze[row][col + 1]) { movement = LEFT; }  // if equal, prioritize turn left
          break;
        case EAST:
          if (refMaze[row - 1][col] > refMaze[row + 1][col]) { movement = RIGHT; }
          if (refMaze[row - 1][col] < refMaze[row + 1][col]) { movement = LEFT; }
          if (refMaze[row - 1][col] == refMaze[row + 1][col]) { movement = LEFT; }  // if equal, prioritize turn left
          break;
        case WEST:
          if (refMaze[row - 1][col] > refMaze[row + 1][col]) { movement = LEFT; }
          if (refMaze[row - 1][col] < refMaze[row + 1][col]) { movement = RIGHT; }
          if (refMaze[row - 1][col] == refMaze[row + 1][col]) { movement = LEFT; }  // if equal, prioritize turn left
          break;
        default:
          {
            movement = LEFT;
            break;
          }
      }
    } else if (!IRFront.status && IRLeft.status && !IRRight.status) {  // obstacle at left
      pathType = STRAIGHT_LEFT;
      switch (direction) {
        case NORTH:
          if (refMaze[row - 1][col] > refMaze[row][col + 1]) { movement = RIGHT; }
          if (refMaze[row - 1][col] < refMaze[row][col + 1]) { movement = FORWARD; }
          if (refMaze[row - 1][col] == refMaze[row][col + 1]) { movement = FORWARD; }  // if equal, prioritize move forward
          break;
        case SOUTH:
          if (refMaze[row + 1][col] > refMaze[row][col - 1]) { movement = RIGHT; }
          if (refMaze[row + 1][col] < refMaze[row][col - 1]) { movement = FORWARD; }
          if (refMaze[row + 1][col] == refMaze[row][col - 1]) { movement = FORWARD; }  // if equal, prioritize move forward
          break;
        case EAST:
          if (refMaze[row][col + 1] > refMaze[row + 1][col]) { movement = RIGHT; }
          if (refMaze[row][col + 1] < refMaze[row + 1][col]) { movement = FORWARD; }
          if (refMaze[row][col + 1] == refMaze[row + 1][col]) { movement = FORWARD; }  // if equal, prioritize move forward
          break;
        case WEST:
          if (refMaze[row][col - 1] > refMaze[row - 1][col]) { movement = RIGHT; }
          if (refMaze[row][col - 1] < refMaze[row - 1][col]) { movement = FORWARD; }
          if (refMaze[row][col - 1] == refMaze[row - 1][col]) { movement = FORWARD; }  // if equal, prioritize move forward
          break;
        default:
          {
            movement = RIGHT;
            break;
          }
      }
    } else if (!IRFront.status && !IRLeft.status && IRRight.status) {  // obstacle at right
      pathType = STRAIGHT_RIGHT;
      switch (direction) {
        case NORTH:
          if (refMaze[row - 1][col] > refMaze[row][col - 1]) { movement = LEFT; }
          if (refMaze[row - 1][col] < refMaze[row][col - 1]) { movement = FORWARD; }
          if (refMaze[row - 1][col] == refMaze[row][col - 1]) { movement = FORWARD; }  // if equal, prioritize move forward
          break;
        case SOUTH:
          if (refMaze[row + 1][col] > refMaze[row][col + 1]) { movement = LEFT; }
          if (refMaze[row + 1][col] < refMaze[row][col + 1]) { movement = FORWARD; }
          if (refMaze[row + 1][col] == refMaze[row][col + 1]) { movement = FORWARD; }  // if equal, prioritize move forward
          break;
        case EAST:
          if (refMaze[row][col + 1] > refMaze[row - 1][col]) { movement = LEFT; }
          if (refMaze[row][col + 1] < refMaze[row - 1][col]) { movement = FORWARD; }
          if (refMaze[row][col + 1] == refMaze[row - 1][col]) { movement = FORWARD; }  // if equal, prioritize move forward
          break;
        case WEST:
          if (refMaze[row][col - 1] > refMaze[row + 1][col]) { movement = LEFT; }
          if (refMaze[row][col - 1] < refMaze[row + 1][col]) { movement = FORWARD; }
          if (refMaze[row][col - 1] == refMaze[row + 1][col]) { movement = FORWARD; }  // if equal, prioritize move forward
          break;
        default:
          {
            movement = FORWARD;
            break;
          }
      }
    } else {  // no obstacle ahead
      pathType = STRAIGHT_LEFT_RIGHT;
      switch (direction) {
        case NORTH:
          if (refMaze[row - 1][col] <= refMaze[row][col - 1] && refMaze[row - 1][col] <= refMaze[row][col + 1]) { movement = FORWARD; }
          if (refMaze[row][col - 1] <= refMaze[row - 1][col] && refMaze[row][col - 1] <= refMaze[row][col + 1]) { movement = LEFT; }
          if (refMaze[row][col + 1] <= refMaze[row - 1][col] && refMaze[row][col + 1] <= refMaze[row][col - 1]) { movement = RIGHT; }
          break;
        case SOUTH:
          if (refMaze[row + 1][col] <= refMaze[row][col - 1] && refMaze[row + 1][col] <= refMaze[row][col + 1]) { movement = FORWARD; }
          if (refMaze[row][col + 1] <= refMaze[row + 1][col] && refMaze[row][col + 1] <= refMaze[row][col - 1]) { movement = LEFT; }
          if (refMaze[row][col - 1] <= refMaze[row + 1][col] && refMaze[row][col - 1] <= refMaze[row][col + 1]) { movement = RIGHT; }
          break;
        case EAST:
          if (refMaze[row][col + 1] <= refMaze[row - 1][col] && refMaze[row][col + 1] <= refMaze[row + 1][col]) { movement = FORWARD; }
          if (refMaze[row - 1][col] <= refMaze[row][col + 1] && refMaze[row - 1][col] <= refMaze[row + 1][col]) { movement = LEFT; }
          if (refMaze[row + 1][col] <= refMaze[row - 1][col] && refMaze[row + 1][col] <= refMaze[row][col + 1]) { movement = RIGHT; }
          break;
        case WEST:
          if (refMaze[row][col - 1] <= refMaze[row - 1][col] && refMaze[row][col - 1] <= refMaze[row + 1][col]) { movement = FORWARD; }
          if (refMaze[row + 1][col] <= refMaze[row - 1][col] && refMaze[row + 1][col] <= refMaze[row][col - 1]) { movement = LEFT; }
          if (refMaze[row - 1][col] <= refMaze[row][col - 1] && refMaze[row - 1][col] <= refMaze[row + 1][col]) { movement = RIGHT; }
          break;
        default:
          {
            movement = FORWARD;
            break;
          }
      }
    }
  }
}

// Execute the locomotion code
void executeMovementCode() {
  // Serial.print("Maze array element: ");
  // Serial.print(myRobot.Maze[row][col]);
  // Serial.print("    ");
  if (myRobot.Maze[row][col] != WALL && myRobot.Maze[row][col] != END && isValid(row, col)) {
    switch (movement) {
      case FORWARD:
        {
          leftWheelObj.moveForward();
          rightWheelObj.moveForward();
          // Serial.println("Forward");
          break;
        }
      case LEFT:
        {
          checkMovement.disable();
          leftPulse = 0;
          // rightPulse = 0;
          while (leftPulse < 11) {
            leftWheelObj.moveLeft();
            rightWheelObj.moveLeft();
            // Serial.println("Left");
          }
          leftPulse = 0;
          // rightPulse = 0;
          if (direction == NORTH) {
            direction = WEST;
          } else if (direction == SOUTH) {
            direction = EAST;
          } else if (direction == WEST) {
            direction = SOUTH;
          } else {
            direction = NORTH;
          }
          checkMovement.enable();
          break;
        }

      case RIGHT:
        {
          checkMovement.disable();
          leftPulse = 0;
          // rightPulse = 0;
          while (leftPulse < 11) {
            leftWheelObj.moveRight();
            rightWheelObj.moveRight();
            // Serial.println("Right");
          }
          leftPulse = 0;
          // rightPulse = 0;
          if (direction == NORTH) {
            direction = EAST;
          } else if (direction == SOUTH) {
            direction = WEST;
          } else if (direction == WEST) {
            direction = NORTH;
          } else {
            direction = SOUTH;
          }
          checkMovement.enable();
          break;
        }
      case BACKWARD:
        {
          leftWheelObj.moveReverse();
          rightWheelObj.moveReverse();
          // Serial.println("Backward");
          if (direction == NORTH) {
            direction = SOUTH;
          } else if (direction == SOUTH) {
            direction = NORTH;
          } else if (direction == WEST) {
            direction = EAST;
          } else {
            direction = WEST;
          }
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
    // Serial.print("Stop");
    // Serial.print("    ");
    // Serial.print(direction);
    // Serial.print("    ");
  }
  // if(myRobot.Maze[row][col] == END) {
  //   leftWheelObj.moveStop();
  //   rightWheelObj.moveStop();
  //    Serial.println("Stop");
  // }
}

// Help map the maze
void mazeMappingCode() {
  myRobot.visited[row][col] = true;
  switch (pathType) {
    case STRAIGHT_LEFT_RIGHT:
      switch (direction) {
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
      switch (direction) {
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
      switch (direction) {
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
      switch (direction) {
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
      switch (direction) {
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
          switch (direction) {
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
          switch (direction) {
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
          switch (direction) {
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

// Get the front grid coordinate
void getFrontGrid() {
  switch (direction) {
    case NORTH:
      frontCellVisited[row][col] = myRobot.Maze[row - 1][col];
      frontCellJunction[row][col] = myRobot.visited[row - 1][col];
      break;
    case SOUTH:
      frontCellVisited[row][col] = myRobot.Maze[row + 1][col];
      frontCellJunction[row][col] = myRobot.visited[row + 1][col];
      break;
    case EAST:
      frontCellVisited[row][col] = myRobot.Maze[row][col + 1];
      frontCellJunction[row][col] = myRobot.visited[row][col + 1];
      break;
    case WEST:
      frontCellVisited[row][col] = myRobot.Maze[row][col - 1];
      frontCellJunction[row][col] = myRobot.visited[row][col - 1];
      break;
    default: break;
  }
}

// Get the left grid coordinate
void getLeftGrid() {
  switch (direction) {
    case NORTH:
      leftCellVisited[row][col] = myRobot.Maze[row][col - 1];
      leftCellJunction[row][col] = myRobot.visited[row][col - 1];
      break;
    case SOUTH:
      leftCellVisited[row][col] = myRobot.Maze[row][col + 1];
      leftCellJunction[row][col] = myRobot.visited[row][col + 1];
      break;
    case EAST:
      leftCellVisited[row][col] = myRobot.Maze[row - 1][col];
      leftCellJunction[row][col] = myRobot.visited[row - 1][col];
      break;
    case WEST:
      leftCellVisited[row][col] = myRobot.Maze[row + 1][col];
      leftCellJunction[row][col] = myRobot.visited[row + 1][col];
      break;
    default: break;
  }
}

// Get the right grid coordinate
void getRightGrid() {
  switch (direction) {
    case NORTH:
      rightCellVisited[row][col] = myRobot.Maze[row][col + 1];
      rightCellJunction[row][col] = myRobot.visited[row][col + 1];
      break;
    case SOUTH:
      rightCellVisited[row][col] = myRobot.Maze[row][col - 1];
      rightCellJunction[row][col] = myRobot.visited[row][col - 1];
      break;
    case EAST:
      rightCellVisited[row][col] = myRobot.Maze[row + 1][col];
      rightCellJunction[row][col] = myRobot.visited[row + 1][col];
      break;
    case WEST:
      rightCellVisited[row][col] = myRobot.Maze[row - 1][col];
      rightCellJunction[row][col] = myRobot.visited[row - 1][col];
      break;
    default: break;
  }
}

void setup() {
  Serial.begin(9600);
  myRobot.initializeMaze();
  initializeWallMaze();
  movement = FORWARD;
  direction = NORTH;
  // Serial.print("Initial Row: "); Serial.print(row); Serial.print("     ");
  // Serial.print("Initial Col: "); Serial.println(col);
  leftWheelObj.declarePin();
  rightWheelObj.declarePin();
  frontSensor.declarePin();
  rightSensor.declarePin();
  leftSensor.declarePin();
  IRFront.declarePin();
  IRLeft.declarePin();
  IRRight.declarePin();
  pinMode(LEFT_ENC, INPUT);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC), leftCounter, RISING);
  pinMode(RIGHT_ENC, INPUT);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC), rightCounter, RISING);

  frontDetect.enable();
  leftDetect.enable();
  rightDetect.enable();
  // GetLeftPulseTask.enable();
  // GetRightPulseTask.enable();
  // checkMovement.enable();
  // executeMovement.enable();
  // updateMaze.enable();
  // frontIRDetect.enable();
  // leftIRDetect.enable();
  // rightIRDetect.enable();
  // mazeMapping.enable();
  // getFront.enable();
  // getLeft.enable();
  // getRight.enable();
  leftAverageVal.enable();
  rightAverageVal.enable();
  PIDTask.enable();
  ForwardTask.enable();
  // TuningTask.enable();
}

void loop() {
  // Serial.print("Left Encoder Pulse: "); Serial.print(leftPulse); Serial.print("    ");
  // Serial.print("Right Encoder Pulse:"); Serial.print(rightPulse); Serial.print("    ");
  // Serial.print("Front Sensor: "); Serial.print(frontSensor.data); Serial.print("    ");
  // Serial.print("Left Sensor: "); Serial.print(leftSensor.data); Serial.print("    ");
  // Serial.print("Right Sensor: "); Serial.print(rightSensor.data); Serial.println("    ");
  // myRobot.testFunctions();
  // myRobot.solveMaze();
  // myRobot.IRsolveMaze();
  frontDetect.check();
  leftDetect.check();
  rightDetect.check();
  // frontIRDetect.check();
  // leftIRDetect.check();
  // rightIRDetect.check();
  // GetLeftPulseTask.check();
  // GetRightPulseTask.check();
  // checkMovement.check();
  // executeMovement.check();
  // updateMaze.check();
  // mazeMapping.check();
  // getFront.check();
  // getLeft.check();
  // getRight.check();
  leftAverageVal.check();
  rightAverageVal.check();
  PIDTask.check();
  ForwardTask.check();
  // TuningTask.check();
}