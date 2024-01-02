// Pin Declaration
#define LEFT_PWM A3
#define LEFT_DIR_1 28
#define LEFT_DIR_2 31
#define RIGHT_PWM A1
#define RIGHT_DIR_1 29
#define RIGHT_DIR_2 30

#define RIGHT_TRIG 8
#define RIGHT_ECHO 9
#define LEFT_TRIG 4
#define LEFT_ECHO 5

#define LEFT_ENC 21
#define LEFT_PIN 19

#define FRONT_IR 52
#define LEFT_IR 48
#define RIGHT_IR 50

// Constants
#define SIZE 13
#define TEST_SIZE 6
#define LINEAR_DELAY 850
#define TURN_DELAY 520

enum {
  FORWARD,
  LEFT,
  RIGHT,
  BACKWARD,
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