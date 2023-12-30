// Pin Declaration
#define LEFT_PWM A3
#define LEFT_DIR_1 29
#define LEFT_DIR_2 31
#define RIGHT_PWM A5
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
#define LEFT_PIN 19
#define RIGHT_PIN 18

#define FRONT_IR 50
#define LEFT_IR 48
#define RIGHT_IR 52

// Constants
#define DIST_THRESH 12
#define SIDE_DIST_THRESH 5
#define PULSE_PER_GRID 24
#define SIZE 12
#define TASK_INTERVAL 1000

enum {
  FORWARD,
  LEFT,
  RIGHT,
  REVERSE,
  STOP
} movement;

enum {
  NORTH,
  SOUTH,
  EAST,
  WEST
} direction;

enum {
  STRAIGHT_LEFT_RIGHT,
  STRAIGHT_LEFT,
  STRAIGHT_RIGHT,
  RIGHT_CORNER,
  LEFT_CORNER,
  T_JUNCTION,
  CROSS_JUNCTION,
  DEAD_END,
} pathType;

#define PATH 0
#define WALL 1
#define START 2
#define END 3 