#ifndef LEFTWHEEL_H_INCLUDED
#define LEFTWHEEL_H_INCLUDED

// Interface class of leftWheel (Inherited from 'wheel' class)
class leftWheel : public wheel
{
  public:
    leftWheel(int, int, int); // Constructor
    void declarePin(); // For pin declaration
    void moveForward(); // For forward movmement
    void reverse(); // For backward movement 
    void moveLeft(); // For turn left movmement
    void moveRight(); // For turn right movement 
    void stop(); // For stopping robot movement
  private:
    int left_pwm; // PWM Pin
    int left_dir_1; // Direction Pin 1
    int left_dir_2; // Direction Pin 2
};

#endif // LEFTWHEEL_H_INCLUDED