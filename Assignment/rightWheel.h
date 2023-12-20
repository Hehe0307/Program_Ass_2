#ifndef RIGHTWHEEL_H_INCLUDED
#define RIGHTWHEEL_H_INCLUDED

// Interface class of rightWheel (Inherited from 'wheel' class)
class rightWheel : public wheel
{
  public:
    rightWheel(int, int, int); // Constructor
    void declarePin(); // For pin declaration
    void moveForward(); // For forward movmement
    void moveReverse(); // For backward movement 
    void moveLeft(); // For turn left movmement
    void moveRight(); // For turn right movement 
    void moveStop(); // For stopping robot movement
  private:
    int right_pwm; // PWM Pin
    int right_dir_1; // Direction Pin 1
    int right_dir_2; // Direction Pin 2
};

#endif // RIGHTWHEEL_H_INCLUDED