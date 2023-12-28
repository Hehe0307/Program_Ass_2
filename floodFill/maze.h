#ifndef MAZE_H_INCLUDED
#define MAZE_H_INCLUDED

#include <TimedAction.h>

using namespace std; 

// Interface class of 'maze'
class maze {
  public:
    maze(leftWheel, rightWheel, ultrasonic, ultrasonic, ultrasonic);
    void testFunctions();
    void solveMaze();
  private:
    leftWheel LeftWheel;
    rightWheel RightWheel;
    ultrasonic Front;
    ultrasonic Left;
    ultrasonic Right;
};

#endif; // MAZE_H_INCLUDED