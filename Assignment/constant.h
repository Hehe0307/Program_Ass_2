// Pin Declaration
#define LEFT_PWM 5
#define LEFT_DIR_1 4
#define LEFT_DIR_2 A0
#define RIGHT_PWM 6
#define RIGHT_DIR_1 7
#define RIGHT_DIR_2 A1

#define FRONT_TRIG 12
#define FRONT_ECHO 13
#define RIGHT_TRIG 8
#define RIGHT_ECHO 9
#define LEFT_TRIG 10
#define LEFT_ECHO 11

#define LEFT_ENC 2
#define RIGHT_ENC 3

#define DIST_THRESH 7;
#define PULSE_PER_GRID 24;
#define TASK_INTERVAL 100000;

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
  PATH, 
  WALL, 
  START,
  END
} state;