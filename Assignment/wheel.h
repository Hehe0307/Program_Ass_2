#ifndef WHEEL_H_INCLUDED
#define WHEEL_H_INCLUDED

// Interface abstract class of 'wheel'
struct wheel
{
    wheel(); // Constructor
    ~wheel(); // Destructor
    virtual void declarePin() = 0; // For pin declaration
    virtual void moveForward() = 0; // For forward movmement
    virtual void reverse() = 0; // For backward movement 
    virtual void moveLeft() = 0; // For turn left movmement
    virtual void moveRight() = 0; // For turn right movement 
    virtual void stop() = 0; // For stopping robot movement
};

#endif // WHEEL_H_INCLUDED