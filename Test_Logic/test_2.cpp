#include <bits/stdc++.h>

using namespace std;

const int LEFT_PWM = 1;
const int LEFT_DIR_1 = 2;
const int LEFT_DIR_2 = 3;
const int RIGHT_PWM = 4;
const int RIGHT_DIR_1 = 5;
const int RIGHT_DIR_2 = 6;

const int TRIG_PIN_F = 7;
const int ECHO_PIN_F = 8;
const int TRIG_PIN_L = 9;
const int ECHO_PIN_L = 10;
const int TRIG_PIN_R = 11;
const int ECHO_PIN_R = 12;

struct wheel {
    wheel();
    ~wheel();
    virtual void forward() = 0;
    virtual void left() = 0;
    virtual void right() = 0; 
    virtual void reverse() = 0;
    virtual void stop() = 0;
};

class leftWheel : public wheel {
    public:
        leftWheel(int, int, int);
        void forward();
        void left();
        void right();
        void reverse();
        void stop();
    private:
        int left_pwm;
        int left_dir_1;
        int left_dir_2;
};

class rightWheel : public wheel {
    public:
        rightWheel(int, int, int);
        void forward();
        void left();
        void right();
        void reverse();
        void stop();
    private:
        int right_pwm;
        int right_dir_1;
        int right_dir_2;
};

class ultrasonic {
    public:
        ultrasonic(int, int);
        void retrieveData();
    private:
        int trigPin;
        int echoPin;
};

class robot {
    public:
        robot(leftWheel, rightWheel, ultrasonic, ultrasonic, ultrasonic);
        void solveMaze();
    private:
        leftWheel wheelLeft;
        rightWheel wheelRight;
        ultrasonic front;
        ultrasonic left;
        ultrasonic right;
};

wheel::wheel() { cout << "A wheel object created" << endl; }
wheel::~wheel() { cout << "A wheel object destroyed" << endl; }

leftWheel::leftWheel(int left_pwm, int left_dir_1, int left_dir_2) 
: left_pwm(left_pwm), left_dir_1(left_dir_1), left_dir_2(left_dir_2) {
    cout << "A left wheel object created" << endl;
}

void leftWheel::forward() { cout << "Left Wheel move forward" << endl; }
void leftWheel::left() { cout << "Left Wheel move left" << endl; }
void leftWheel::right() { cout << "Left Wheel move right" << endl; }
void leftWheel::reverse() { cout << "Left Wheel move reverse" << endl; }
void leftWheel::stop() { cout << "Left Wheel move stop" << endl; }

rightWheel::rightWheel(int right_pwm, int right_dir_1, int right_dir_2)
: right_pwm(right_pwm), right_dir_1(right_dir_1), right_dir_2(right_dir_2) {
    cout << "A right wheel object created" << endl;
}

void rightWheel::forward() { cout << "Right Wheel move forward" << endl; }
void rightWheel::left() { cout << "Right Wheel move left" << endl; }
void rightWheel::right() { cout << "Right Wheel move right" << endl; }
void rightWheel::reverse() { cout << "Right Wheel move reverse" << endl; }
void rightWheel::stop() { cout << "Right Wheel move stop" << endl; }

ultrasonic::ultrasonic(int trigPin, int echoPin) 
: trigPin(trigPin), echoPin(echoPin) {
    cout << "A HC-SR04 object created" << endl;
}

void ultrasonic::retrieveData() { cout << "Retrieve Data" << endl; }

robot::robot(leftWheel wheelLeft, rightWheel wheelRight, ultrasonic front, ultrasonic left, ultrasonic right) 
: wheelLeft(wheelLeft), wheelRight(wheelRight), front(front), left(left), right(right) {
    cout << "A robot object created" << endl;
}

void robot::solveMaze() {
    wheelLeft.forward();
    wheelLeft.left();
    wheelLeft.right();
    wheelLeft.reverse();
    wheelLeft.stop();

    wheelRight.forward();
    wheelRight.left();
    wheelRight.right();
    wheelRight.reverse();
    wheelRight.stop();

    front.retrieveData();
    left.retrieveData();
    right.retrieveData();
}

int main() {
    leftWheel leftWheelObj(LEFT_PWM, LEFT_DIR_1, LEFT_DIR_2);
    rightWheel rightWheelObj(RIGHT_PWM, RIGHT_DIR_1, RIGHT_DIR_2);
    ultrasonic frontObj(TRIG_PIN_F, ECHO_PIN_F);
    ultrasonic leftObj(TRIG_PIN_L, ECHO_PIN_L);
    ultrasonic rightObj(TRIG_PIN_R, ECHO_PIN_R);

    robot myRobot(leftWheelObj, rightWheelObj, frontObj, leftObj, rightObj);
    myRobot.solveMaze();

    return 0;
}