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
    virtual void turnLeft() = 0; // To be used in child classes 'leftWheel' and 'rightWheel' 
    virtual void turnRight() = 0; // To be used in child classes 'leftWheel' and 'rightWheel' 
  private:
    const int wheel_pwm; // PWM Pin
    const int wheel_dir1; // Direction Pin1
    const int wheel_dir2; // Direction Pin2
};

#endif // WHEEL_H_INCLUDED