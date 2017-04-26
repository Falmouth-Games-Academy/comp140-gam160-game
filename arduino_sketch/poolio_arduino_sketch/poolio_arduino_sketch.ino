/*
 * For the accelerometer, used library and adapted source code found at https://learn.sparkfun.com/tutorials/adxl345-hookup-guide
*/

#include <SparkFun_ADXL345.h>

ADXL345 adxl = ADXL345();

//Digital pins
const int JoystickSwitchPin = 4;
const int ButtonPin = 2;

//Analog pins
const int JoystickXPin = 2;
const int JoystickYPin = 3;
const int PotentiometerPin = 1;

String OutputString;

void setup() {
  Serial.begin(9600);
  
  pinMode(ButtonPin, INPUT);

  adxl.powerOn();       
  adxl.setRangeSetting(16);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

  adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library 
   
  adxl.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
 
  adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?
}

void loop() {

  // Accelerometer Readings
  int x,y,z;   
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z
  
  OutputString = String(String("s") +     // Arbitary letter "s" indicates start of output string for parsing
  "," + String(digitalRead(ButtonPin)) +
  "," + String(analogRead(JoystickXPin)) +
  "," + String(analogRead(PotentiometerPin)) +
  "," + String(x) +
  ",");

  Serial.print(OutputString);
}

