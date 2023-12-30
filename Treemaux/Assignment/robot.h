#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

// Interface class of robot
class robot
{
  public:
    robot(ultrasonic f, ultrasonic r, ultrasonic l, leftWheel lw, rightWheel rw, IR fir, IR lir, IR rir); // Constructor
    void testFunctions();
    void solveMaze();
    void IRsolveMaze();
    void initializeMaze();
    int Maze[12][12] = { 0 }; // Initiallize the maze with 0's
    bool visited[12][12] = { false }; // false as not visited, true as visited

  private:
    // Robot 'has a' Front, Right, Left, LeftWheel, RightWheel (Composition)
    ultrasonic Front; // Front HC-SR04 sensor
    ultrasonic Right; // Right HC-SR04 sensor
    ultrasonic Left; // Left HC-SR04 sensor
    leftWheel LeftWheel; 
    rightWheel RightWheel; 
    IR FrontIR;
    IR LeftIR;
    IR RightIR;
};

#endif