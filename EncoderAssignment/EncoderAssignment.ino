#include "Arduino.h"
#include "constant.h"
#include "encoder.h"

#include <TimedAction.h>

void GetLeftPulseTaskCode();
void GetRightPulseTaskCode();
TimedAction GetLeftPulseTask = TimedAction(100000, GetLeftPulseTaskCode);
TimedAction GetRightPulseTask = TimedAction(100000, GetRightPulseTaskCode);
Encoder leftEncoder(2);
Encoder rightEncoder(3);
volatile uint32_t leftPulse = 0;
volatile uint32_t rightPulse = 0;

void GetLeftPulseTaskCode()
{
  cli();
  sei();
}

void GetRightPulseTaskCode()
{
  cli();
  sei();
}

void leftCounter()
{
    leftPulse++;
}

void rightCounter()
{
  rightPulse++;
}

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), leftCounter, RISING);
  attachInterrupt(digitalPinToInterrupt(3), rightCounter, RISING);
  GetLeftPulseTask.enable();
  GetRightPulseTask.enable();
}

void loop() {
  Serial.print("Left Encoder Pulse: ");
  Serial.print(leftPulse);
  Serial.print("    ");
  Serial.print("Right Encoder Pulse:");
  Serial.print(rightPulse);
  Serial.println();
  
  GetLeftPulseTask.check();
  GetRightPulseTask.check();
}