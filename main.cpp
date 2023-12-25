#include <TimedAction.h>
#include <cmath>

// Pin Declaration
#define LEFT_PWM 27
#define LEFT_DIR_1 29
#define LEFT_DIR_2 31
#define RIGHT_PWM 26
#define RIGHT_DIR_1 28
#define RIGHT_DIR_2 30

#define FRONT_TRIG 4
#define FRONT_ECHO 5
#define RIGHT_TRIG 6
#define RIGHT_ECHO 7
#define LEFT_TRIG 8
#define LEFT_ECHO 9

#define LEFT_ENC 21
#define RIGHT_ENC 20

// Constants
#define DIST_THRESH 7
#define PULSE_PER_GRID 24
#define TASK_INTERVAL 100
#define SIZE 12

// Direction
enum {
  NORTH, EAST, SOUTH, WEST
} direction;

// Movement
enum {
  FORWARD, LEFT, RIGHT, REVERSE, STOP
} movement;

// Interface abstract class of 'wheel'
class wheel
{
  public:
    wheel();
    virtual void declarePin() = 0; // For pin declaration
    virtual void moveForward() = 0; // For forward movmement
    virtual void moveReverse() = 0; // For backward movement 
    virtual void moveStop() = 0; // For stopping robot movement
    virtual void moveLeft() = 0; // For turn left movmement
    virtual void moveRight() = 0; // For turn right movement 
};

// Interface class of leftWheel (Inherited from 'wheel' class)
class leftWheel : public wheel
{
  public:
    leftWheel(const int, const int, const int); // Constructor
    void declarePin(); // For pin declaration
    void moveLeft(); // For turn left movmement
    void moveRight(); // For turn right movement 
    void moveForward(); // For forward movmement
    void moveReverse(); // For backward movement 
    void moveStop(); // For stopping robot movement
  private:
    const int left_pwm; // PWM Pin
    const int left_dir_1; // Direction Pin 1
    const int left_dir_2; // Direction Pin 2
};

// Interface class of rightWheel (Inherited from 'wheel' class)
class rightWheel : public wheel
{
  public:
    rightWheel(const int, const int, const int); // Constructor 
    void declarePin(); // For pin declaration
    void moveLeft(); // For turn left movmement
    void moveRight(); // For turn right movement 
    void moveForward(); // For forward movmement
    void moveReverse(); // For backward movement 
    void moveStop(); // For stopping robot movement
  private:
    const int right_pwm; // PWM Pin
    const int right_dir_1; // Direction Pin 1
    const int right_dir_2; // Direction Pin 2
};

// Interface class of ultrasonic
class ultrasonic
{
  public:
    ultrasonic(const int, const int); // Constructor
    void declarePin(); // For pin declaration
    long retrieveData(); // Retrieve sensor readings
  private:
    const int trigPin; // Trigger Pin
    const int echoPin; // Echo Pin
};

// Implementation of 'wheel' class
wheel::wheel() {}

// Implementation of 'leftWheel' class
leftWheel::leftWheel(const int left_pwm, const int left_dir_1, const int left_dir_2) 
: left_pwm(left_pwm), left_dir_1(left_dir_1), left_dir_2(left_dir_2) {}

void leftWheel::declarePin() {
  wheel::wheel(left_pwm, left_dir_1, left_dir_2);
}

void leftWheel::moveForward() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir_1, HIGH);
  digitalWrite(left_dir_2, LOW);
}

void leftWheel::moveReverse() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir_1, LOW);
  digitalWrite(left_dir_2, HIGH);
}

void leftWheel::moveStop() {
  digitalWrite(left_pwm, LOW);
  digitalWrite(left_dir_1, HIGH);
  digitalWrite(left_dir_2, LOW);
}
 
void leftWheel::moveLeft() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir_1, LOW);
  digitalWrite(left_dir_2, HIGH);
}

void leftWheel::moveRight() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir_1, HIGH);
  digitalWrite(left_dir_2, LOW);
}

// Implementation of 'rightWheel' class
rightWheel::rightWheel(const int right_pwm, const int right_dir_1, const int right_dir_2) 
: right_pwm(right_pwm), right_dir_1(right_dir_1), right_dir_2(right_dir_2) {}

void rightWheel::declarePin() {
  wheel::wheel(right_pwm, right_dir_1, right_dir_2);
}

void rightWheel::moveForward() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir_1, HIGH);
  digitalWrite(right_dir_2, LOW);
}

void rightWheel::moveReverse() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir_1, LOW);
  digitalWrite(right_dir_2, HIGH);
}

void rightWheel::moveStop() {
  digitalWrite(right_pwm, LOW);
  digitalWrite(right_dir_1, HIGH);
  digitalWrite(right_dir_2, LOW);
}

void rightWheel::moveLeft() {
    digitalWrite(right_pwm, HIGH);
    digitalWrite(right_dir_1, HIGH);
    digitalWrite(right_dir_2, LOW);
}

void rightWheel::moveRight() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir_1, LOW);
  digitalWrite(right_dir_2, HIGH);
}

// Implementation of 'ultrasonic' class
ultrasonic::ultrasonic(const int trigPin, const int echoPin) : trigPin(trigPin), echoPin(echoPin) {}

void ultrasonic::declarePin() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long ultrasonic::retrieveData()
{
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // delays are required for a succesful sensor operation.
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // this delay is required as well!
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return (duration / 58); // convert the distance to centimeters.
}

leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR_1, LEFT_DIR_2);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR_1, RIGHT_DIR_2);
ultrasonic frontSensor(FRONT_TRIG, FRONT_ECHO);
ultrasonic leftSensor(LEFT_TRIG, LEFT_ECHO);
ultrasonic rightSensor(RIGHT_TRIG, RIGHT_ECHO);

void GetLeftPulseTaskCode();
void GetRightPulseTaskCode();
void checkMovementCode();
void executeMovementCode();
void updateMazeCode();
void checkNextMoveCode();

TimedAction GetLeftPulseTask = TimedAction(TASK_INTERVAL, GetLeftPulseTaskCode);
TimedAction GetRightPulseTask = TimedAction(TASK_INTERVAL, GetRightPulseTaskCode);
TimedAction checkMovement = TimedAction(TASK_INTERVAL, checkMovementCode);
TimedAction executeMovement = TimedAction(TASK_INTERVAL, executeMovementCode);
TimedAction updateMaze = TimedAction(TASK_INTERVAL, updateMazeCode);
TimedAction checkNextMove = TimedAction(TASK_INTERVAL, checkNextMoveCode);

volatile uint32_t leftPulse = 0;
volatile uint32_t rightPulse = 0;
int row, col = 0;
int maze[SIZE][SIZE] = {
  { 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10 },
  { 9, 8, 7, 6, 5, 4, 4, 5, 6, 7, 8, 9 },
  { 8, 7, 6, 5, 4, 3, 3, 4, 5, 6, 7, 8 },
  { 7, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 7 },
  { 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6 },
  { 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5 },
  { 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5 },
  { 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6 },
  { 7, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 7 },
  { 8, 7, 6, 5, 4, 3, 3, 4, 5, 6, 7, 8 },
  { 9, 8, 7, 6, 5, 4, 4, 5, 6, 7, 8, 9 },
  { 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10 }
}
int nextMove = 10;

void GetLeftPulseTaskCode() { cli(); sei(); }
void GetRightPulseTaskCode() { cli(); sei(); }
void leftCounter() { leftPulse++; }
void rightCounter() { rightPulse++; }

void checkMovementCode() {
  long leftVal = leftSensor.retrieveData();
  long rightVal = rightSensor.retrieveData();
  if(frontSensor.retrieveData() < DIST_THRESH) {
    switch(direction) {
      case NORTH: { 
        maze[row-1][col]++;
        if(leftVal < DIST_THRESH && rightVal > DIST_THRESH) { maze[row][col-1]++; movement = RIGHT; }
        else if(leftVal > DIST_THRESH && rightVal < DIST_THRESH) { maze[row][col+1]++; movement = LEFT; }
        else { maze[row][col-1]++; maze[row][col+1]++; movement = REVERSE; } 
        break; 
      }
      case EAST: { 
        maze[row][col+1]++; 
        if(leftVal < DIST_THRESH && rightVal > DIST_THRESH) { maze[row-1][col]++; movement = RIGHT; }
        else if(leftVal > DIST_THRESH && rightVal < DIST_THRESH) { maze[row+1][col]++; movement = LEFT; }
        else { maze[row-1][col]++; maze[row+1][col]++; movement = REVERSE; } 
        break; 
      }
      case SOUTH: { 
        maze[row+1][col]++; 
        if(leftVal < DIST_THRESH && rightVal > DIST_THRESH) { maze[row][col+1]++; movement = RIGHT; }
        else if(leftVal > DIST_THRESH && rightVal < DIST_THRESH) { maze[row][col-1]++; movement = LEFT; }
        else { maze[row][col-1]++; maze[row][col+1]++; movement = REVERSE; } 
        break;
      }
      case WEST: { 
        maze[row][col-1]++; 
        if(leftVal < DIST_THRESH && rightVal > DIST_THRESH) { maze[row+1][col]++; movement = RIGHT; }
        else if(leftVal > DIST_THRESH && rightVal < DIST_THRESH) { maze[row-1][col]++; movement = LEFT; }
        else { maze[row-1][col]++; maze[row+1][col]++; movement = REVERSE; } 
        break; 
      }
    }
  }
}

void executeMovementCode() {
  if(maze[row][col] != 0) {
    switch(movement) {
      case FORWARD:
        leftWheelObj.moveForward();
        rightWheelObj.moveForward();
      case LEFT:
        switch(direction) {
          case NORTH: { direction = WEST; break; }
          case SOUTH: { direction = EAST; break; }
          case WEST: { direction = SOUTH; break; }
          case EAST: { direction = NORTH; break; }
          default: { break; }
        }
        checkMovement.disable();
        leftPulse = 0;
        rightPulse = 0;
        while(leftPulse < 11 && rightPulse < 11) {
          leftWheelObj.moveLeft();
          rightWheelObj.moveLeft();
        }
        leftPulse = 0;
        rightPulse = 0;
        checkMovement.enable();
      case RIGHT:
        switch(direction) {
          case NORTH: { direction = EAST; break; }
          case SOUTH: { direction = WEST; break; }
          case WEST: { direction = NORTH; break; }
          case EAST: { direction = SOUTH; break; }
          default: { break; }
        }
        checkMovement.disable();
        leftPulse = 0;
        rightPulse = 0;
        while(leftPulse < 11 && rightPulse < 11) {
          leftWheelObj.moveRight();
          rightWheelObj.moveRight();
        }
        leftPulse = 0;
        rightPulse = 0;
        checkMovement.enable();
      case REVERSE:
        switch(direction) {
          case NORTH: { direction = SOUTH; break; }
          case SOUTH: { direction = NORTH; break; }
          case WEST: { direction = EAST; break; }
          case EAST: { direction = WEST; break; }
          default: { break; }
        }
        checkMovement.disable();
        leftPulse = 0;
        rightPulse = 0;
        while(leftPulse < 21 && rightPulse < 21) {
          leftWheelObj.moveReverse();
          rightWheelObj.moveReverse();
        }
        leftPulse = 0;
        rightPulse = 0;
        checkMovement.enable();
      default:
        break;
    }
  } else { 
    leftWheelObj.moveStop();
    rightWheelObj.moveStop();
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

void checkNextMoveCode() {
  nextMove = maze[row-1][col]; // NORTH
  if(maze[row+1][col] < nextMove) { nextMove = maze[row+1][col]; } // SOUTH
  if(maze[row][col-1] < nextMove) { nextMove = maze[row][col-1]; } // WEST
  if(maze[row][col+1] < nextMove) { nextMove = maze[row][col+1]; } // EAST
}

void setup() { 
  Serial.begin(115200);
  leftWheelObj.declarePin();
  rightWheelObj.declarePin();
  frontSensor.declarePin();
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
  Serial.print("Left Encoder Pulse: "); Serial.print(leftPulse); Serial.print("    ");
  Serial.print("Right Encoder Pulse:"); Serial.println(rightPulse);

  GetLeftPulseTask.check();
  GetRightPulseTask.check();
  checkMovement.check();
  executeMovement.check();
  updateMaze.check();
}