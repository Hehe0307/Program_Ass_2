#ifndef IR_H_INCLUDED
#define IR_H_INCLUDED

// Interface class of IR
class IR {
  public:
    IR(const int outputPin); // Constructor
    void declarePin(); // For pin declaration
    void retrieveData(); // Retrieve sensor data
    bool status;
  private:
    const int outputPin; // Output Pin
};

#endif // IR_H_INCLUDED