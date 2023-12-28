#ifndef WHEEL_H_INCLUDED
#define WHEEL_H_INCLUDED

// Interface abstract class of 'wheel'
class wheel
{
  public:
    wheel();
    wheel(const int wheel_pwm,  const int wheel_dir_1, const int wheel_dir_2, int speed); // Constructor
    virtual void declarePin(); // For pin declaration
    virtual void moveForward(); // For forward movmement
    virtual void moveStop(); // For stopping robot movement
    virtual void moveReverse(); // For reverse movement 
    virtual void moveLeft() = 0; // For turn left movmement
    virtual void moveRight() = 0; // For turn right movement
    int speed; // Motor Speed 
  private:
    const int wheel_pwm; // PWM Pin
    const int wheel_dir_1; // Direction Pin 1
    const int wheel_dir_2; // Direction Pin 2
};

#endif // WHEEL_H_INCLUDED