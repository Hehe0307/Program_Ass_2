#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

// Interface class of robot
class robot {
  public:
    robot(leftWheel lw, rightWheel rw, IR fir, IR lir, IR rir); // Constructor
    void testFunctions(); // Test functions for locomotion
    void IRsolveMaze(); // Simple left-wall follower 
    void forwardMovement(); // forward movement
    void leftMovement(); // left movement
    void rightMovement(); // right movement
    void backwardMovement(); // backward movement
    void stopMovement(); // stop movement
    void declareAllPins(); // pin declaration
  private:
    // Robot 'has a' LeftWheel, RightWheel, Front IR, Left IR, Right IR (Composition)
    leftWheel LeftWheel; // Left Wheel
    rightWheel RightWheel; // Right Wheel
    IR FrontIR; // Front IR sensor
    IR LeftIR; // Left IR sensor
    IR RightIR; // Right IR sensor
};

#endif