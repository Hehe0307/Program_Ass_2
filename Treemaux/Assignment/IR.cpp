#include "IR.h"
#include "Arduino.h"

IR::IR(const int outputPin) : outputPin(outputPin) {}

void IR::declarePin() { pinMode(outputPin, INPUT); }

void IR::retrieveData() { 
  int val = digitalRead(outputPin); 
  if (val == 0) { status = true; } // Got obstacle
  else { status = false; } // No obstacle
}