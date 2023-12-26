#include "wheel.h"
#include "leftWheel.h"
#include "rightWheel.h"
#include "ultrasonic.h"
#include "encoder.h"
#include "maze.h"
#include "constant.h"

#include "Arduino.h"
#include <queue>

maze::maze(leftWheel LeftWheel, rightWheel RightWheel, ultrasonic Front, ultrasonic Left, ultrasonic Right, encoder leftEncoder, encoder rightEncoder)
: _leftWheel(LeftWheel), _rightWheel(RightWheel), _front(Front), _left(Left), _right(Right), _left_enc(leftEncoder), _right_enc(rightEncoder) {}

void maze::printMaze() {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            Serial.print(Maze[i][j]); Serial.print(" ");
        }
        Serial.println("");
    }
}

void maze::checkNextMoveCode() {

}

void maze::checkWallCode() {
    long frontData = _front.retrieveData();
    long leftData = _left.retrieveData();
    long rightData = _right.retrieveData();
    if(frontData < DIST_THRESH) { 
        switch(direction) {
            case NORTH: wall = NORTH_WALL; break;
            case SOUTH: wall = SOUTH_WALL; break;
            case EAST: wall = EAST_WALL; break;
            case WEST: wall = WEST_WALL; break;
        }
    }
    if(leftData < DIST_THRESH) { 
        switch(direction) {
            case NORTH: wall = WEST_WALL; break;
            case SOUTH: wall = EAST_WALL; break;
            case EAST: wall = NORTH_WALL; break;
            case WEST: wall = SOUTH_WALL; break;
        }
    }
    if(rightData < DIST_THRESH) { 
        switch(direction) {
            case NORTH: wall = EAST_WALL; break;
            case SOUTH: wall = WEST_WALL; break;
            case EAST: wall = SOUTH_WALL; break;
            case WEST: wall = NORTH_WALL; break;
        }
    }
}

void maze::queueOpenNeighbour() {

}

void maze::floodFillCode(int maze[SIZE][SIZE], int startRow, int startCol) {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(maze[i][j] != 0) {
                maze[i][j] = -1; // Blank state
            }
        }
    }

    maze[startRow][startCol] = 0;
    q.push({startRow, startCol});
}

void initializeMaze() {
    Maze[SIZE][SIZE] = {
        { 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10 },
        { 9, 8, 7, 6, 5, 4, 4, 5, 6, 7, 8, 9 },
        { 8, 7, 6, 5, 4, 3, 3, 4, 5, 6, 7, 8 },
        { 7, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 7 },
        { 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6 },
        { 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5 },
        { 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5 },
        { 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6 },
        { 7, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 7 },
        { 8, 7, 6, 5, 4, 3, 3, 4, 5, 6, 7, 8 },
        { 9, 8, 7, 6, 5, 4, 4, 5, 6, 7, 8, 9 },
        { 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10 }
    }
}

void maze::checkTask() {
    checkNextMove.check();
    checkWall.check();
    floodFill.check();
}

void maze::enableTask() {
    checkNextMove.enable();
    checkWall.enable();
    floodFill.enable();
}