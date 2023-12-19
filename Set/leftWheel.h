#ifndef LEFTWHEEL_H_INCLUDED
#define LEFTWHEEL_H_INCLUDED

// Interface class of leftWheel (Inherited from 'wheel' class)
class leftWheel : public wheel
{
  public:
    leftWheel(const int, const int, const int); // Constructor
    void turnLeft(); // Implementation of pure virtual function in parent class 'wheel'
    void turnRight(); // Implementation of pure virtual function in parent class 'wheel'
  private:
    const int left_pwm; // PWM Pin
    const int left_dir1; // Direction Pin
    const int left_dir2;
};

#endif // LEFTWHEEL_H_INCLUDED