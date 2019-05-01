
/*
    MPU6050 Triple Axis Gyroscope & Accelerometer. Pitch & Roll & Yaw Gyroscope Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-i-akcelerometr-mpu6050.html
    GIT: https://github.com/jarzebski/Arduino-MPU6050
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Timers
unsigned long timer = 0;
float timeStep = 0.0003;

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
String val;
float rate_gyr_x;
float rate_gyr_y;
float rate_gyr_z;
float gyroXangle;
float gyroYangle;
float gyroZangle;
float CFangleX;
float CFangleY;
float CFangleZ;


void setup() 
{
  //setting up rate of bits per minute
  Serial.begin(9600);
  //setting up the button pin
  pinMode(button1Pin, INPUT);
  
  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
  }
  
  
  // Calibrate gyroscope
  mpu.calibrateGyro();

  // Set threshold sensivty
  mpu.setThreshold(3);

  //Serial.println("Finished setup");
}

void loop()
{
  timer = millis();

//If new data is detected run code below
if(Serial.available() > 0){
    //The variable incomingByte contains the incoming infromation
    incomingByte = Serial.read();
    
    //if incomingByte is equal to the character of p 
   //then the arduino will send the information from the mpu6050
    if(incomingByte == 'p')
    {
      sendData();
    }

    //If the incomingByte is equal to r then pitch,roll and yaw are set to 0.0
    if(incomingByte == 'r')
    {
      pitch = 0.0;
      roll = 0.0;
      yaw = 0.0;
    }

    //When the button is pressed send e to the game else send 0
    if(digitalRead(button1Pin) == HIGH)
    {
      val = 'e';
    }
    else
    {
      val = '0';
    }
    
}

  // Read normalized values of the gyroscope and accelerometer
  Vector norm = mpu.readNormalizeGyro();
  Vector axis = mpu.readNormalizeAccel();

  // Calculate Pitch, Roll and Yaw
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;

  //set variables to the acceloration from the 
  X = axis.XAxis;
  Y = axis.YAxis;
  Z = axis.ZAxis;

}

void sendData()
{
  //The code below send the data from the GyroScope via the specifc port and creates a new line after val
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
