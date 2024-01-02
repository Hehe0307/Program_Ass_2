#ifndef ULTRASONIC_H_INCLUDED
#define ULTRASONIC_H_INCLUDED

// Interface class of ultrasonic
class ultrasonic
{
  public:
    ultrasonic(const int trigPin, const int echoPin); // Constructor
    void declarePin(); // For pin declaration
    void retrieveData(); // Retrieve sensor readings
    long data;
  private:
    const int trigPin; // Trigger Pin
    const int echoPin; // Echo Pin
};

#endif // ULTRASONIC_H_INCLUDED