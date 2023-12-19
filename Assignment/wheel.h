#ifndef WHEEL_H_INCLUDED
#define WHEEL_H_INCLUDED

// Interface class of wheel
class wheel
{
  public:
    wheel(const int, const int, const int); // Constructor
    void moveForward(); // For forward movmement
    void reverse(); // For rotating 180 degree movement 
    void stop(); // For stopping robot movement
  private:
    const int wheel_pwm; // PWM Pin
    const int wheel_dir_1; // Direction Pin 1
    const int wheel_dir_2; // Direction Pin 2
};

#endif // WHEEL_H_INCLUDED