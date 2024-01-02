// Group 10
// Members: Eng Chee Wen 22050762/1  Lee Yi Jia 22004570/1  Lai Zhe Jun S2182269/1  Chong Wei Jie 22054152/1

#include "constant.h"
#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "IR.h"
#include "robot.h"

// Object Declaration
leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR_1, LEFT_DIR_2, MOTOR_SPEED);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR_1, RIGHT_DIR_2, MOTOR_SPEED);
IR IRFront(FRONT_IR);
IR IRLeft(LEFT_IR);
IR IRRight(RIGHT_IR);
robot myRobot(leftWheelObj, rightWheelObj, IRFront, IRLeft, IRRight);

int row = 5;
int col = 0;

int Maze[SIZE][SIZE];
bool visited[SIZE][SIZE];

bool No_Wall[SIZE][SIZE];
bool North_Wall[SIZE][SIZE];
bool South_Wall[SIZE][SIZE];
bool East_Wall[SIZE][SIZE];
bool West_Wall[SIZE][SIZE];
// const int refMaze[SIZE][SIZE] = {
//   { 12, 11, 10, 9, 8, 7, 6, 7, 8, 9, 10, 11, 12 },
//   { 11, 10, 9, 8, 7, 6, 5, 6, 7, 8, 9, 10, 11 },
//   { 10, 9, 8, 7, 6, 5, 4, 5, 6, 7, 8, 9, 10 },
//   { 9, 8, 7, 6, 5, 4, 3, 4, 5, 6, 7, 8, 9 },
//   { 8, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 8 },
//   { 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7 },
//   { 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6 },
//   { 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7 },
//   { 8, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 8 },
//   { 9, 8, 7, 6, 5, 4, 3, 4, 5, 6, 7, 8, 9 },
//   { 10, 9, 8, 7, 6, 5, 4, 5, 6, 7, 8, 9, 10 },
//   { 11, 10, 9, 8, 7, 6, 5, 6, 7, 8, 9, 10, 11 },
//   { 12, 11, 10, 9, 8, 7, 6, 7, 8, 9, 10, 11, 12 }
// };
const int refMaze[TEST_SIZE][TEST_SIZE] = {
{4, 3, 2, 2, 3, 4},
{3, 2, 1, 1, 2, 3},
{2, 1, 0, 0, 1, 2},
{2, 1, 0, 0, 1, 2},
{3, 2, 1, 1, 2, 3},
{4, 3, 2, 2, 3, 4}
};

bool isValid(int row, int col) { return (row >= 0 && col >= 0 && row < TEST_SIZE && col < TEST_SIZE); }
// bool isValid(int row, int col) { return (row >= 0 && col >= 0 && row < SIZE && col < SIZE); }

void initializeMaze() {
  for (int i = 0; i < TEST_SIZE; i++) {
    for (int j = 0; j < TEST_SIZE; j++) {
      Maze[i][j] = 0; // Initialize each element of the Maze to 0
      visited[i][j] = false; // false as not visited, true as visited
      No_Wall[i][j] = false;
      North_Wall[i][j] = false;
      South_Wall[i][j] = false;
      East_Wall[i][j] = false;
      West_Wall[i][j] = false;
    }
  }
}
// void initializeMaze() {
//   for (int i = 0; i < SIZE; i++) {
//     for (int j = 0; j < SIZE; j++) {
//       Maze[i][j] = 0; // Initialize each element of the Maze to 0
//       visited[i][j] = false; // false as not visited, true as visited
//       No_Wall[i][j] = false;
//       North_Wall[i][j] = false;
//       South_Wall[i][j] = false;
//       East_Wall[i][j] = false;
//       West_Wall[i][j] = false;
//     }
//   }
// }

void checkMovement() {
  if (isValid(row, col)) {
    IRFront.retrieveData();
    IRLeft.retrieveData();
    IRRight.retrieveData();
    if (IRFront.status && IRLeft.status && IRRight.status) {  // dead end
      movement = BACKWARD;
      pathType = DEAD_END;
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
        default: {
          movement = FORWARD;
          break;
        }
      }
    }
  }
}

void executeMovement() {
  Serial.print("Maze array element: ");
  Serial.print(Maze[row][col]);
  Serial.print("    ");
  switch (movement) {
    case FORWARD: {
      myRobot.forwardMovement();
      // Serial.println("Forward");
      delay(LINEAR_DELAY);
      myRobot.stopMovement();
      // Serial.println("Stop");
      if (direction == NORTH) { row--; } 
      else if (direction == SOUTH) { row++; } 
      else if (direction == WEST) { col--; } 
      else if (direction = EAST) { col++; } 
      break;
    }
    case LEFT: {
      myRobot.leftMovement();
      // Serial.println("Left");
      delay(TURN_DELAY);
      myRobot.forwardMovement();
      // Serial.println("Forward");
      delay(LINEAR_DELAY);
      myRobot.stopMovement();
      // Serial.println("Stop");
      if (direction == NORTH) { direction = WEST; col--; } 
      else if (direction == SOUTH) { direction = EAST; col++; } 
      else if (direction == WEST) { direction = SOUTH; row++; } 
      else if (direction = EAST) { direction = NORTH; row--; }
      break;
    }
    case RIGHT: {
      myRobot.rightMovement();
      // Serial.println("Right");
      delay(TURN_DELAY);
      myRobot.forwardMovement();
      // Serial.println("Forward");
      delay(LINEAR_DELAY);
      myRobot.stopMovement();
      // Serial.println("Stop");
      if (direction == NORTH) { direction = EAST; col++; } 
      else if (direction == SOUTH) { direction = WEST; col--; } 
      else if (direction == WEST) { direction = NORTH; row--; } 
      else if (direction == EAST) { direction = SOUTH; row++; }
      break;
    }
    case BACKWARD: {
      myRobot.backwardMovement();
      // Serial.println("Backward");
      delay(LINEAR_DELAY);
      myRobot.stopMovement();
      // Serial.println("Stop");
      if (direction == NORTH) { row++; } 
      else if (direction == SOUTH) { row--; } 
      else if (direction == WEST) { col++; } 
      else if (direction == EAST) { col--; }
      break;
    }
    default: {
      myRobot.stopMovement();
      // Serial.println("Stop");
      break;
    }
  }
}

void markWall() {
  visited[row][col] = true;
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
      }
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initializeMaze();
  movement = FORWARD;
  direction = NORTH;
  Serial.print("Initial Row: "); Serial.print(row); Serial.print("     ");
  Serial.print("Initial Col: "); Serial.println(col);
  myRobot.declareAllPins();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(refMaze[row][col] != 0) {
    checkMovement(); delay(500);
    markWall(); delay(500);
    executeMovement(); delay(2000);
  }
  if(refMaze[row][col] == 0) {
    myRobot.stopMovement();
    Serial.println("End");
  }
}
