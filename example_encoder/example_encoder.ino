/// Variables //////////////////////////////////////////////////////////////////////////////////////////////////////////////
int encoder_pin = 2;             // Pin 2, where the encoder is connected       
unsigned int rpm = 0;           // Revolutions per minute calculated.
float velocity = 0;                  // Velocity in [Km/h]
volatile byte pulses = 0;       // Number of pulses read by the Arduino in one second
unsigned long timeold = 0;  // Old time 
unsigned int pulsesperturn = 20; // Number of notches on the encoder disk.
const int wheel_diameter = 47;   // Diameter of the small wheel [mm]
static volatile unsigned long debounce = 0; // De bounce time.
//// Arduino Configuration /////////////////////////////////////////////////////////
void setup(){
   Serial.begin(9600); // Serial port configuration  
   pinMode(encoder_pin, INPUT); // Pin number 2 configuration
   attachInterrupt(0, counter, RISING); // Configuration of interrupt 0, where it's connected.
   pulses = 0;
   rpm = 0;
   timeold = 0;
  Serial.print("Seconds ");
  Serial.print("RPM ");
  Serial.print("Pulses ");
  Serial.println("Velocity[Km/h]");
}
//// Main Program ///////////////////////////////////////////////////////////////////////
void loop(){
   if (millis() - timeold >= 1000){  // Update every second
      noInterrupts(); // Don't process interrupts during calculations // Turn off interrupt processing to perform calculations.
      rpm = (60 * 1000 / pulsesperturn )/ (millis() - timeold)* pulses; // Calculate revolutions per minute
      velocity = rpm * 3.1416 * wheel_diameter * 60 / 1000000; // Calculate velocity in [Km/h] 
      timeold = millis(); // Store the current time.
      Serial.print(millis()/1000); Serial.print("       ");// Send time value, RPM, and pulses to the serial port.
      Serial.print(rpm,DEC); Serial.print("   ");
      Serial.print(pulses,DEC); Serial.print("     ");
      Serial.println(velocity,2); 
      pulses = 0;  // Initialize pulses.
      interrupts(); // Restart the interrupt processing // Restart interrupt processing
   }
}
//// End of main program //////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Function that counts valid pulses ///////////////////////////////////////////
void counter(){
  if(  digitalRead (encoder_pin) && (micros()-debounce > 500) && digitalRead (encoder_pin) ) { 
// Check again if the encoder sends a valid signal and then check if the time is greater than 1000 microseconds, and verify again that the signal is correct.
        debounce = micros(); // Store the time to check for bounce in the signal.
        pulses++;}  // Add a valid pulse.
        else ; } 
