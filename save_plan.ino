#include "Arduino.h"

// Pin Declaration
#define LEFT_PWM 5
#define LEFT_DIR 4
#define RIGHT_PWM 6
#define RIGHT_DIR 7

#define FRONT_TRIG 2
#define FRONT_ECHO 3
#define RIGHT_TRIG 8
#define RIGHT_ECHO 9
#define LEFT_TRIG 10
#define LEFT_ECHO 11

// Interface class of wheel
class wheel
{
  public:
    wheel(const int, const int); // Constructor
    void moveForward(); // For forward movmement
    void reverse(); // For rotating 180 degree movement 
    void stop(); // For stopping robot movement
    virtual void turnLeft() = 0; // To be used in child classes 'leftWheel' and 'rightWheel' 
    virtual void turnRight() = 0; // To be used in child classes 'leftWheel' and 'rightWheel' 
  private:
    const int wheel_pwm; // PWM Pin
    const int wheel_dir; // Direction Pin
};

// Interface class of leftWheel (Inherited from 'wheel' class)
class leftWheel : public wheel
{
  public:
    leftWheel(const int, const int); // Constructor
    void turnLeft(); // Implementation of pure virtual function in parent class 'wheel'
    void turnRight(); // Implementation of pure virtual function in parent class 'wheel'
  private:
    const int left_pwm; // PWM Pin
    const int left_dir; // Direction Pin
};

// Interface class of rightWheel (Inherited from 'wheel' class)
class rightWheel : public wheel
{
  public:
    rightWheel(const int, const int); // Constructor
    void turnLeft(); // Implementation of pure virtual function in parent class 'wheel'
    void turnRight(); // Implementation of pure virtual function in parent class 'wheel'
  private:
    const int right_pwm;
    const int right_dir;
};

// Interface class of ultrasonic
class ultrasonic
{
  public:
    ultrasonic(const int, const int); // Constructor
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
wheel::wheel(const int wheel_pwm, const int wheel_dir) : wheel_pwm(wheel_pwm), wheel_dir(wheel_dir) {
  pinMode(wheel_pwm, OUTPUT);
  pinMode(wheel_dir, OUTPUT);
}

void wheel::moveForward() {
  digitalWrite(wheel_pwm, HIGH);
  digitalWrite(wheel_dir, LOW);
}

void wheel::reverse() {
  digitalWrite(wheel_pwm, LOW);
  digitalWrite(wheel_dir, HIGH);
}

void wheel::stop() {
  digitalWrite(wheel_pwm, LOW);
  digitalWrite(wheel_dir, LOW);
}

// Implementation of 'leftWheel' class
leftWheel::leftWheel(const int left_pwm, const int left_dir) : wheel::wheel(left_pwm, left_dir) {}

void leftWheel::turnLeft() {
  digitalWrite(left_pwm, HIGH);
  digitalWrite(left_dir, LOW);
}

void leftWheel::turnRight() {
  digitalWrite(left_pwm, LOW);
  digitalWrite(left_dir, HIGH);
}

// Implementation of 'rightWheel' class
rightWheel::rightWheel(const int right_pwm, const int right_dir) : wheel::wheel(right_pwm, right_dir) {}

void rightWheel::turnLeft() {
  digitalWrite(right_pwm, LOW);
  digitalWrite(right_dir, HIGH);
}

void rightWheel::turnRight() {
  digitalWrite(right_pwm, HIGH);
  digitalWrite(right_dir, LOW);
}

// Implementation of 'ultrasonic' class
ultrasonic::ultrasonic(const int trigPin, const int echoPin) : trigPin(trigPin), echoPin(echoPin)
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long ultrasonic::retrieveData()
{
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // delays are required for a successful sensor operation.
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // this delay is required as well!
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return (duration / 58); // convert the distance  to centimeters.
}

// Implementation of 'robot' class
robot::robot(ultrasonic f, ultrasonic r, ultrasonic l, leftWheel lw, rightWheel rw) 
: Front(f), Right(r), Left(l), LeftWheel(lw), RightWheel(rw) {}

void robot::solveMaze() {
  const int dist_thresh = 5;
  while (true) {
    // Your sensor readings and movement logic here
    // Use Front.retrieveData(), Right.retrieveData(), and Left.retrieveData() for sensor readings
    // Use LeftWheel.moveForward(), RightWheel.moveForward(), LeftWheel.turnLeft(), RightWheel.turnLeft(),
    // LeftWheel.turnRight(), RightWheel.turnRight() for robot movements

    if (Front.retrieveData() > dist_thresh && Right.retrieveData() > dist_thresh && Left.retrieveData() > dist_thresh) { // no obstacles ahead  
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Forward");
      // delay(100);
    }
    else if ( Front.retrieveData() > dist_thresh && Right.retrieveData() < dist_thresh && Left.retrieveData() < dist_thresh) { // left & right have obstacles but front is clear
      LeftWheel.moveForward();
      RightWheel.moveForward(); 
      // Serial.println("Forward");
      // delay(100);
    }
    else if ( Front.retrieveData() < dist_thresh && Right.retrieveData() < dist_thresh && Left.retrieveData() < dist_thresh) { // obstacle infront of all 3 sides, reverse
      LeftWheel.reverse();
      RightWheel.reverse();
      // Serial.println("Reverse");
      delay(500);
      if((Left.retrieveData()) > (Right.retrieveData())) { 
        LeftWheel.turnLeft();
        RightWheel.turnLeft();
        // Serial.println("Left");
        // delay(100); 
      }
      else { 
        LeftWheel.turnRight();
        RightWheel.turnRight();
        // Serial.println("Right"); 
        // delay(100);
      }
    }
    else if (Front.retrieveData() < dist_thresh && Right.retrieveData() < dist_thresh && Left.retrieveData() > dist_thresh) { // obstacle on right and front sides, turn left side 
      LeftWheel.turnLeft(); 
      RightWheel.turnLeft(); 
      // Serial.println("Left");
      // delay(100);
    }
    else if (Front.retrieveData() < dist_thresh && Right.retrieveData() > dist_thresh && Left.retrieveData() < dist_thresh) { // obstacle on left and front sides, turn right side
      LeftWheel.turnRight();
      RightWheel.turnRight();
      // Serial.println("Right");
      // delay(100);
    }
    else  if (Front.retrieveData() < dist_thresh && Right.retrieveData() > dist_thresh && Left.retrieveData() > dist_thresh) { // obstacle on front side, turn left & then forward
      LeftWheel.turnLeft();
      LeftWheel.turnLeft(); 
      delay(500); 
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Left then Forward"); 
      // delay(100);
    }
    else if (Front.retrieveData() > dist_thresh && Right.retrieveData() > dist_thresh && Left.retrieveData() < dist_thresh) { // obstacle on left side, turn right & then forward
      LeftWheel.turnRight();
      RightWheel.turnRight();
      delay(500);
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Right then Forward");
      // delay(100);
    }
    else if (Front.retrieveData() > dist_thresh && Right.retrieveData() < dist_thresh && Left.retrieveData() > dist_thresh) { // obstacle on right side, turn left & then forward 
      LeftWheel.turnLeft();
      RightWheel.turnLeft();
      delay(500);
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Left then Forward");
      // delay(100);
    }
    else { 
      LeftWheel.moveForward();
      RightWheel.moveForward();
      // Serial.println("Forward");
      // delay(100);
    }
  }
}

leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR);
rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR);
ultrasonic frontSensor(FRONT_TRIG, FRONT_ECHO);
ultrasonic rightSensor(RIGHT_TRIG, RIGHT_ECHO);
ultrasonic leftSensor(LEFT_TRIG, LEFT_ECHO);
robot myRobot(frontSensor, rightSensor, leftSensor, leftWheelObj, rightWheelObj);

void setup() { 
  Serial.begin(115200);
}

void loop()
{
  Serial.println("Start");
  myRobot.solveMaze();
  Serial.println("End");
  delay(100);
}