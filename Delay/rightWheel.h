#ifndef RIGHTWHEEL_H_INCLUDED
#define RIGHTWHEEL_H_INCLUDED

// Interface class of rightWheel (Inherited from 'wheel' class)
class rightWheel : public wheel {
  public:
    rightWheel(const int right_pwm, const int right_dir_1, const int right_dir_2, int speed); // Constructor 
    void declarePin(); // For pin declaration
    void moveLeft(); // For turn left movmement
    void moveRight(); // For turn right movement 
    void moveForward(); // For forward movmement
    void moveReverse(); // For backward movement 
    void moveStop(); // For stopping robot movement
    int speed;
  private:
    const int right_pwm; // PWM Pin
    const int right_dir_1; // Direction Pin 1
    const int right_dir_2; // Direction Pin 2
};

#endif // RIGHTWHEEL_H_INCLUDED