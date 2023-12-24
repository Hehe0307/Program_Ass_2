// Pin Declaration
#define LEFT_PWM 5
#define LEFT_DIR_1 4
#define LEFT_DIR_2 A0
#define RIGHT_PWM 6
#define RIGHT_DIR_1 7
#define RIGHT_DIR_2 A1

#define FRONT_TRIG 12
#define FRONT_ECHO 13
#define RIGHT_TRIG 8
#define RIGHT_ECHO 9
#define LEFT_TRIG 10
#define LEFT_ECHO 11

#define LEFT_ENC 2
#define RIGHT_ENC 3

#define DIST_THRESH 7

// Interface abstract class of 'wheel'
class wheel
{
  public:
    wheel();
    wheel(int wheel_pwm, int wheel_dir_1, int wheel_dir_2); // Constructor
    virtual void declarePin(); // For pin declaration
    virtual void moveForward(); // For forward movmement
    virtual void moveReverse(); // For backward movement 
    virtual void moveStop(); // For stopping robot movement
    virtual void moveLeft() = 0; // For turn left movmement
    virtual void moveRight() = 0; // For turn right movement 
  private:
    int wheel_pwm;
    int wheel_dir_1;
    int wheel_dir_2;
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

// Interface class of robot
class robot
{
  public:
    robot(ultrasonic, ultrasonic, ultrasonic, leftWheel, rightWheel); // Constructor
    void testFunctions();
    void solveMaze();
  private:
    // Robot 'has a' Front, Right, Left, LeftWheel, RightWheel (Composition)
    ultrasonic Front; // Front HC-SR04 sensor
    ultrasonic Right; // Right HC-SR04 sensor
    ultrasonic Left; // Left HC-SR04 sensor
    leftWheel LeftWheel; 
    rightWheel RightWheel; 
};

// Implementation of 'wheel' class
wheel::wheel() {}

wheel::wheel(const int wheel_pwm, const int wheel_dir_1, const int wheel_dir_2) 
: wheel_pwm(wheel_pwm), wheel_dir_1(wheel_dir_1), wheel_dir_2(wheel_dir_2) {}

void wheel::declarePin() {
  pinMode(wheel_pwm, OUTPUT);
  pinMode(wheel_dir_1, OUTPUT);
  pinMode(wheel_dir_2, OUTPUT);
}

void wheel::moveForward() {
  digitalWrite(wheel_pwm, HIGH);
  digitalWrite(wheel_dir_1, HIGH);
  digitalWrite(wheel_dir_2, LOW);
}

void wheel::moveReverse() {
  digitalWrite(wheel_pwm, HIGH);
  digitalWrite(wheel_dir_1, LOW);
  digitalWrite(wheel_dir_2, HIGH);
}

void wheel::moveStop() {
  digitalWrite(wheel_pwm, LOW);
  digitalWrite(wheel_dir_1, HIGH);
  digitalWrite(wheel_dir_2, LOW);
}

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
  return (duration / 58); // convert the distance  to centimeters.
}

// Implementation of 'robot' class
robot::robot(ultrasonic f, ultrasonic r, ultrasonic l, leftWheel lw, rightWheel rw) 
: Front(f), Right(r), Left(l), LeftWheel(lw), RightWheel(rw) {}

void robot::testFunctions() {
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

void robot::solveMaze() {
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

leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR_1, LEFT_DIR_2);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR_1, RIGHT_DIR_2);
ultrasonic frontSensor(FRONT_TRIG, FRONT_ECHO);
ultrasonic rightSensor(RIGHT_TRIG, RIGHT_ECHO);
ultrasonic leftSensor(LEFT_TRIG, LEFT_ECHO);
robot myRobot(frontSensor, rightSensor, leftSensor, leftWheelObj, rightWheelObj);

void setup() { 
  Serial.begin(115200);
  leftWheelObj.declarePin();
  rightWheelObj.declarePin();
  frontSensor.declarePin();
  rightSensor.declarePin();
  leftSensor.declarePin();
}

void loop() {
  // myRobot.testFunctions();
  myRobot.solveMaze();
}