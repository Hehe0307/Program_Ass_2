// Pin Declaration
#define LEFT_PWM 27
#define LEFT_DIR_1 29
#define LEFT_DIR_2 31
#define RIGHT_PWM 26
#define RIGHT_DIR_1 28
#define RIGHT_DIR_2 30

#define FRONT_TRIG 4
#define FRONT_ECHO 5
#define RIGHT_TRIG 6
#define RIGHT_ECHO 7
#define LEFT_TRIG 8
#define LEFT_ECHO 9

#define LEFT_ENC 21
#define RIGHT_ENC 20

// Constants
#define DIST_THRESH 7
#define PULSE_PER_GRID 24
#define TASK_INTERVAL 100
#define SIZE 12

enum {
  FORWARD, LEFT, RIGHT, REVERSE, STOP
} movement;

enum {
  NORTH, SOUTH, EAST, WEST
} direction;

enum {
  NORTH_WALL, SOUTH_WALL, EAST_WALL, WEST_WALL
} wall;