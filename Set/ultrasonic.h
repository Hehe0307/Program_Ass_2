#ifndef ULTRASONIC_H_INCLUDED
#define ULTRASONIC_H_INCLUDED

// Interface class of ultrasonic
class ultrasonic
{
  public:
    ultrasonic(const int, const int); // Constructor
    void declarePin(); // For pin declaration
    long retrieveData(); // Retrieve sensor readings
  private:
    const int trigPin; // Trigger Pin
    const int echoPin; // Echo Pin
};

#endif // ULTRASONIC_H_INCLUDED