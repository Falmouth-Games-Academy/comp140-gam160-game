// Basic demo for accelerometer readings from Adafruit LIS3DH

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10

// software SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// hardware SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
// I2C
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#define FLEXSENSORIN A3
#define FLEXSENSOROUT 2

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef int8_t int8;
typedef int16_t int16;

struct ArduinoToPCData {
  // Start data stream marker
  uint16 startMarker; //=0xADDE
  
  // Accelerometer values
  int16 accelX;
  int16 accelY;
  int16 accelZ;

  // Raw bend sensor value between 0 and 1024
  uint16 flexValue;

  // End data stream marker
  uint16 endMarker; //=0x0DD0
};

void setup(void) {
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif

  Serial.begin(19200);
  Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  
  Serial.print("Range = "); Serial.print(2 << lis.getRange());
  Serial.println("G");

  // Setup bend sensor
  pinMode(FLEXSENSORIN, INPUT);
  pinMode(FLEXSENSOROUT, OUTPUT);
  digitalWrite(FLEXSENSOROUT, true);
}

void loop() {
  //lis.read();      // get X Y and Z data at once
  // Then print out the raw data
  /*Serial.print("X:  "); Serial.print(lis.x); 
  Serial.print("  \tY:  "); Serial.print(lis.y); 
  Serial.print("  \tZ:  "); Serial.print(lis.z); */

  /* Or....get a new sensor event, normalized */ 
  sensors_event_t event; 
  lis.getEvent(&event);
  
  /* Display the results (acceleration is measured in m/s^2) */
  /*Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  Serial.print(" \tY: "); Serial.print(event.acceleration.y); 
  Serial.print(" \tZ: "); Serial.print(event.acceleration.z); 
  Serial.println(" m/s^2 ");

  Serial.println();*/

  // Send the data back to the PC through the serial port
  ArduinoToPCData output;

  output.startMarker = 0xADDE;
  output.accelX = (int)(event.acceleration.x * 1000);
  output.accelY = (int)(event.acceleration.y * 1000);
  output.accelZ = (int)(event.acceleration.z * 1000);
  output.flexValue = analogRead(FLEXSENSORIN);
  output.endMarker = 0x0DD0;
  
  Serial.write((char*)&output, sizeof (output));
}
