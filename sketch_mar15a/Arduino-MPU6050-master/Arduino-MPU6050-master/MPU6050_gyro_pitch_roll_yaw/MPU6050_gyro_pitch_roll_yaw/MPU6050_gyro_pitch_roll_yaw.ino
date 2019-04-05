/*
    MPU6050 Triple Axis Gyroscope & Accelerometer. Pitch & Roll & Yaw Gyroscope Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-i-akcelerometr-mpu6050.html
    GIT: https://github.com/jarzebski/Arduino-MPU6050
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <MPU6050.h>
#include <Adafruit_NeoPixel.h>

MPU6050 mpu;

// Timers
unsigned long timer = 0;
float timeStep = 0.003;

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
float yaw = 0;
float xGyro = 0;
float yGyro = 0;
float zGyro = 0;
int X = 0;
int Y = 0;
int Z = 0;
int num;
int incomingByte;
const int button1Pin = 3;
const int button2Pin = 4;
const int button3Pin = 5;
const int button4Pin = 6;
const int button5Pin = 7;
String val;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, 6, NEO_GRB + NEO_KHZ800);

void setup() 
{
  Serial.begin(9600);
  pinMode(button1Pin, INPUT);
  strip.begin();
  
  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
  }
  
  
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  mpu.setThreshold(3);

  //Serial.println("Finished setup");
}

void loop()
{
  timer = millis();
  
  
  
if(Serial.available() > 0){
    //The variable incomingByte contains the incoming infromation
    incomingByte = Serial.read();

    //if incomingByte is equal to the character of p 
   //then the arduino will send the analog information of poteniometers
    if(incomingByte == 'p')
    {
      sendData();
    }

    if(incomingByte == 'q')
    {
      num +=5;
    }
    
    if(digitalRead(button1Pin) == HIGH)
    {
      val = 'e';
    }
}

  // Read normalized values
  Vector norm = mpu.readNormalizeGyro();
  Vector axis = mpu.readNormalizeAccel();

  // Calculate Pitch, Roll and Yaw
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;
  

  /*pitch = -(atan2(axis.XAxis, sqrt(axis.YAxis*axis.YAxis + axis.ZAxis*axis.ZAxis))*180.0)/M_PI;
  roll = (atan2(axis.YAxis, axis.ZAxis)*180.0)/M_PI;
*/
  
  X = axis.XAxis;
  Y = axis.YAxis;
  Z = axis.ZAxis;

  chase(num);
  
  // Output raw
  /*
  Serial.print(" Pitch = ");
  Serial.print(pitch);
  Serial.print(" Roll = ");
  Serial.print(roll);  
  Serial.print(" Yaw = ");
  Serial.println(yaw);

  // Wait to full timeStep period
  delay((timeStep*1000) - (millis() - timer));
  */
}

void sendData()
{
  //The code below send the data fromt the poteniometers via the specifc port and create a new line
  //if displayed on the serial moniter
  Serial.print(pitch);
  Serial.print(",");
  Serial.print(roll);
  Serial.print(",");
  Serial.print(yaw);
  Serial.print(",");
  Serial.print(X);
  Serial.print(",");
  Serial.print(Y);
  Serial.print(",");
  Serial.print(Z);
  Serial.print(",");
  Serial.println(val);
}

void chase(int num) {
  for(int i=0; i<strip.numPixels()+num; i++) {
      strip.setPixelColor(i  , strip.Color(0, 0, 255)); // Draw new pixel
      
  }
  strip.show();
}


