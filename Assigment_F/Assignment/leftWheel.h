#ifndef LEFTWHEEL_H_INCLUDED
#define LEFTWHEEL_H_INCLUDED

// Interface class of leftWheel (Inherited from 'wheel' class)
class leftWheel : public wheel
{
  public:
    leftWheel(const int left_pwm, const int left_dir_1, const int left_dir_2, int speed); // Constructor
    void declarePin(); // For pin declaration
    void moveLeft(); // For turn left movmement
    void moveRight(); // For turn right movement
    void moveReverse(); // For reverse movement 
    void moveForward(); // For forward movmement
    void moveStop(); // For stopping robot movement
    int speed; // Motor speed
  private:
    const int left_pwm; // PWM Pin
    const int left_dir_1; // Direction Pin 1
    const int left_dir_2; // Direction Pin 2
};

#endif // LEFTWHEEL_H_INCLUDED