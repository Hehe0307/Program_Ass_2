#ifndef RIGHTWHEEL_H_INCLUDED
#define RIGHTWHEEL_H_INCLUDED

// Interface class of rightWheel (Inherited from 'wheel' class)
class rightWheel : public wheel
{
  public:
    rightWheel(const int, const int, const int); // Constructor
    void turnLeft(); // Implementation of pure virtual function in parent class 'wheel'
    void turnRight(); // Implementation of pure virtual function in parent class 'wheel'
  private:
    const int right_pwm;
    const int right_dir1;
    const int right_dir2;
};

#endif // RIGHTWHEEL_H_INCLUDED