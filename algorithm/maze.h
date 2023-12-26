#ifndef MAZE_H_INCLUDED
#define MAZE_H_INCLUDED

#include <TimedAction.h>
#include "constant.h"

using namespace std; 

// Interface class of 'maze'
class maze {
    public:
        maze(leftWheel, rightWheel, ultrasonic, ultrasonic, ultrasonic, encoder, encoder, Queue, Queue);
        int Maze[SIZE][SIZE] = { 0 };
        int horizontalWall[SIZE][SIZE] = { 0 }; // 1 for North Wall, 3 for South Wall
        int verticalWall[SIZE][SIZE] = { 0 }; // 4 for West Wall, 2 for East Wall
        int min(int, int);
        void pidLeftWheelCode();
        void pidRightWheelCode();
        void initializeWallState();
        void testFunctions();
        void solveMaze();
        void updateMazeCode();
        void executeMovementCode();
        void choosePathCode();
        void printMaze();
        bool isValid();
        void checkNextMoveCode();
        void checkAvailableCode();
        void checkWallCode();
        void floodFillCode();
        void checkTask();
        void enableTask();
        TimedAction pidLeftWheel = TimedAction(TASK_INTERVAL, pidLeftWheelCode);
        TimedAction pidRightWheel = TimedAction(TASK_INTERVAL, pidRightWheelCode);
        TimedAction updateMaze = TimedAction(TASK_INTERVAL, updateMazeCode);
        TimedAction executeMovement = TimedAction(TASK_INTERVAL, executeMovementCode);
        TimedAction choosePath = TimedAction(TASK_INTERVAL, choosePathCode);
        TimedAction checkWall = TimedAction(TASK_INTERVAL, checkWallCode);
        TimedAction checkAvailable = TimedAction(TASK_INTERVAL, checkAvailableCode);
        TimedAction checkNextMove = TimedAction(TASK_INTERVAL, checkNextMoveCode);
        TimedAction floodFill = TimedAction(TASK_INTERVAL, floodFillCode);

    private:
        leftWheel LeftWheel;
        rightWheel RightWheel;
        ultrasonic Front;
        ultrasonic Left;
        ultrasonic Right;
        encoder LeftEnc;
        encoder RightEnc;
        Queue q1;
        Queue q2;
        int row = 11;
        int col = 0;
};

#endif; // MAZE_H_INCLUDED