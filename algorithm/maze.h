#ifndef MAZE_H_INCLUDED
#define MAZE_H_INCLUDED

#include <TimedAction.h>
#include <queue>
#include "constant.h"

using namespace std; 

// Interface class of 'maze'
class maze {
    public:
        maze(leftWheel, rightWheel, ultrasonic, ultrasonic, ultrasonic, encoder, encoder);
        int Maze[12][12] = { 0 };
        bool visited[12][12] = { false }
        void initializeMaze();
        void printMaze();
        void checkNextMoveCode();
        void checkWallCode();
        void floodFillCode();
        void checkTask();
        void enableTask();
        TimedAction checkNextMove = TimedAction(TASK_INTERVAL, checkNextMoveCode);
        TimedAction checkWall = TimedAction(TASK_INTERVAL, checkWallCode);
        TimedAction floodFill = TimedAction(TASK_INTERVAL, floodFillCode);
    private:
        leftWheel _leftWheel;
        rightWheel _rightWheel;
        ultrasonic _front;
        ultrasonic _left;
        ultrasonic _right;
        encoder _left_enc;
        encoder _right_enc;
        int row, col = 0;
        int dr[] = { -1, 1, 0, 0 };
        int dc[] = { 0, 0, -1, 1 };
        queue<pair<int, int>> q;
};

#endif; // MAZE_H_INCLUDED