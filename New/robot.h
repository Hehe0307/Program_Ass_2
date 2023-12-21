#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

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

#endif