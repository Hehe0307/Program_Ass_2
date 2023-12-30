#ifndef WHEEL_H_INCLUDED
#define WHEEL_H_INCLUDED

// Interface abstract class of 'wheel'
class wheel
{
  public:
    wheel();
    wheel(int wheel_pwm, int wheel_dir_1, int wheel_dir_2, int speed); // Constructor
    virtual void declarePin(); // For pin declaration
    virtual void moveForward(); // For forward movmement
    virtual void moveReverse(); // For backward movement 
    virtual void moveStop(); // For stopping robot movement
    virtual void moveLeft() = 0; // For turn left movmement
    virtual void moveRight() = 0; // For turn right movement 
    int speed;
  private:
    int wheel_pwm;
    int wheel_dir_1;
    int wheel_dir_2;
};

#endif // WHEEL_H_INCLUDED