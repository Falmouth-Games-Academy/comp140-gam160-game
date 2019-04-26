
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

float AA = 0.97;
float G_GAIN = 0.070;

Vector rawGyro;
Vector rawAccel;

void setup() 
{
  Serial.begin(9600);
  pinMode(button1Pin, INPUT);
  
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
  rawGyro = mpu.readRawGyro();
  rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();
  
  float DT = 0.02;
  

  //Convert Gyro raw to degrees per second
  rate_gyr_x = rawGyro.XAxis * G_GAIN;
  rate_gyr_y = rawGyro.YAxis * G_GAIN;
  rate_gyr_z = rawGyro.ZAxis * G_GAIN;

  //Calculate the angles from the gyro
  gyroXangle+=rate_gyr_x*DT;
  gyroYangle+=rate_gyr_y*DT;
  gyroZangle+=rate_gyr_z*DT;

  //Convert Accelerometer values to degrees
  float AccXangle = (float) (atan2(rawAccel.YAxis,rawAccel.ZAxis)+M_PI)*(180/3.14159);
  float AccYangle = (float) (atan2(rawAccel.ZAxis,rawAccel.XAxis)+M_PI)*(180/3.14159);
  float AccZangle = (float) (atan2(rawAccel.XAxis,rawAccel.ZAxis)+M_PI)*(180/3.14159);

/*
  //If IMU is up the correct way, use these lines
        AccXangle -= (float)180.0;
  if (AccYangle > 90)
          AccYangle -= (float)270;
  else
    AccYangle += (float)90;
*/

  //Complementary filter used to combine the accelerometer and gyro values.
  CFangleX=AA*(CFangleX+rate_gyr_x*DT) +(1 - AA) * AccXangle;
  CFangleY=AA*(CFangleY+rate_gyr_y*DT) +(1 - AA) * AccYangle;
  CFangleZ=AA*(CFangleZ+rate_gyr_z*DT) +(1 - AA) * AccZangle;
  
if(Serial.available() > 0){
    //The variable incomingByte contains the incoming infromation
    incomingByte = Serial.read();
    
    //if incomingByte is equal to the character of p 
   //then the arduino will send the analog information of poteniometers
    if(incomingByte == 'p')
    {
      sendData();
    }

    if(incomingByte == 'r')
    {
      pitch = 0.0;
      roll = 0.0;
      yaw = 0.0;
    }
    
    if(digitalRead(button1Pin) == HIGH)
    {
      val = 'e';
    }
    else
    {
      val = '0';
    }
    
}

  // Read normalized values
  Vector norm = mpu.readNormalizeGyro();
  Vector axis = mpu.readNormalizeAccel();

  // Calculate Pitch, Roll and Yaw
 
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;
  

  /*pitch = 180 * atan (rawAccel.XAxis/sqrt(rawAccel.YAxis*rawAccel.YAxis + rawAccel.ZAxis*rawAccel.ZAxis))/M_PI;
  roll = 180 * atan (rawAccel.YAxis/sqrt(rawAccel.XAxis*rawAccel.XAxis + rawAccel.ZAxis*rawAccel.ZAxis))/M_PI;
  yaw = 180 * atan (rawAccel.ZAxis/sqrt(rawAccel.XAxis*rawAccel.XAxis + rawAccel.ZAxis*rawAccel.ZAxis))/M_PI;
  */


  
  X = axis.XAxis;
  Y = axis.YAxis;
  Z = axis.ZAxis;

  
  
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
