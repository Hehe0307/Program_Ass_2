#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

// Interface class of robot
class robot
{
  public:
    robot(ultrasonic, ultrasonic, ultrasonic, leftWheel, rightWheel); // Constructor
    void testFunctions();
    void solveMaze();
    void initializeMaze();
    void initialNewMaze();
    int Maze[12][12] = { 0 }; // Initiallize the maze with 0's
    bool visited[12][12] = { false }; // 0 as not visited, 1 as visited
  private:
    // Robot 'has a' Front, Right, Left, LeftWheel, RightWheel (Composition)
    ultrasonic Front; // Front HC-SR04 sensor
    ultrasonic Right; // Right HC-SR04 sensor
    ultrasonic Left; // Left HC-SR04 sensor
    leftWheel LeftWheel; 
    rightWheel RightWheel; 
};

#endif