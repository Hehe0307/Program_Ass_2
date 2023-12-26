#ifndef RIGHTWHEEL_H_INCLUDED
#define RIGHTWHEEL_H_INCLUDED

// Interface class of rightWheel (Inherited from 'wheel' class)
class rightWheel : public wheel
{
  public:
    rightWheel(const int, const int, const int); // Constructor 
    void declarePin(); // For pin declaration
    void moveLeft(); // For turn left movmement
    void moveRight(); // For turn right movement 
    void moveForward(); // For forward movmement 
    void moveStop(); // For stopping robot movement
  private:
    const int right_pwm; // PWM Pin
    const int right_dir_1; // Direction Pin 1
    const int right_dir_2; // Direction Pin 2
};

#endif // RIGHTWHEEL_H_INCLUDED