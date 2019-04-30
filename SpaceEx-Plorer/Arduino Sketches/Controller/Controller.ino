// Used for reading gyroscope values
# include <Wire.h>

// Potentiometer port
const int player = A0;

// Clamped value between 0 and 1023 (inclusive)
int playerSpeed = 0;

// Number of leds
const int LEDS = 1;
// Array for led ports
const int leds[LEDS] = {5};
bool ledsOn[LEDS] = {false};

// Gyroscope specific values
int gyro_error = 0;           // Used for
int16_t Gyr_rawX, Gyr_rawY, Gyr_rawZ;     // Raw gyro values
float Gyro_angle_x, Gyro_angle_y, Gyro_angle_z; // Processed angles
float Gyro_raw_error_x, Gyro_raw_error_y, Gyro_raw_error_z;       // Initial gyro error 

// Time specific values
float elapsedTime, time, timePrev;       

long debounce = 200;
long switchTime = 0;

// Button variables
int inPin = 2;
int outPin = 13;

int state = HIGH;
int reading = 0;
int previous = LOW;

int incomingByte = 0;

void setup() {
  // Wire setup
  Wire.begin();                 // begin communication

  Wire.beginTransmission(0x68); // send slave data (68)
  Wire.write(0x6B);             // perform reset
  Wire.write(0x00);
  Wire.endTransmission(true);   // end transmission

  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);
  pinMode(leds[0], OUTPUT);
  
  // Serial setup
  Serial.begin(9600);
  time=millis();                // Start counting time in milliseconds

  if(gyro_error==0)
  {
    for(int i=0; i<200; i++)
    {
      Wire.beginTransmission(0x68);            //begin, Send the slave adress (in this case 68) 
      Wire.write(0x43);                        //First adress of the Gyro data
      Wire.endTransmission(false);
      Wire.requestFrom(0x68,4,true);           //We ask for just 4 registers 
         
      Gyr_rawX=Wire.read()<<8|Wire.read();     //Once again we shift and sum
      Gyr_rawY=Wire.read()<<8|Wire.read();
      Gyr_rawZ=Wire.read()<<8|Wire.read();     //TODO: check functionality
      
      /*---X---*/
      Gyro_raw_error_x = Gyro_raw_error_x + (Gyr_rawX/32.8); 
      /*---Y---*/
      Gyro_raw_error_y = Gyro_raw_error_y + (Gyr_rawY/32.8);
      /*---Z---*/
      Gyro_raw_error_z = Gyro_raw_error_z + (Gyr_rawZ/32.8);    //TODO: check functionality
      if(i==199)
      {
        Gyro_raw_error_x = Gyro_raw_error_x/200;
        Gyro_raw_error_y = Gyro_raw_error_y/200;
        Gyro_raw_error_z = Gyro_raw_error_z/200;                //TODO: check functionality
        gyro_error=1;
      }
    }
  } 
}

// Reset score and such
void resetGame()
{
  
}

// Update specific LED's based on incoming byte
void updateDashboard(int inByte)
{
  if(inByte == 'P')
  {
    ledsOn[0] = true;
  }
}

// Read input from potentiometer and gyro
void getInput()
{

}

void loop() {
  timePrev = time;                        // the previous time is stored before the actual time read
  time = millis();                        // actual time read
  elapsedTime = (time - timePrev) / 1000; //divide by 1000 in order to obtain seconds

  if(incomingByte == 'P')
  {
    digitalWrite(5, HIGH);
  }
  else
  {
    digitalWrite(5, LOW);
  }

  reading = digitalRead(inPin);
  if(reading == HIGH && previous == LOW && millis() - switchTime > debounce)
  {
    if(state == HIGH)
      state = LOW;
    else
      state = HIGH;

    switchTime = millis();
  }

  digitalWrite(outPin, state);
  previous = reading;

  //////////////////////////////////////Gyro read/////////////////////////////////////

    Wire.beginTransmission(0x68);            //begin, Send the slave adress (in this case 68) 
    Wire.write(0x43);                        //First adress of the Gyro data
    Wire.endTransmission(false);
    Wire.requestFrom(0x68,6,true);           //We ask for 6 registers
        
    Gyr_rawX=Wire.read()<<8|Wire.read();     //Once again we shift and sum
    Gyr_rawY=Wire.read()<<8|Wire.read();
    Gyr_rawZ=Wire.read()<<8|Wire.read();     //TODO: check functionality
    /*Now in order to obtain the gyro data in degrees/seconds we have to divide first
    the raw value by 32.8 because that's the value that the datasheet gives us for a 1000dps range*/
    /*---X---*/
    Gyr_rawX = (Gyr_rawX/32.8) - Gyro_raw_error_x; 
    /*---Y---*/
    Gyr_rawY = (Gyr_rawY/32.8) - Gyro_raw_error_y;
    /*---Z---*/
    Gyr_rawZ = (Gyr_rawZ/32.8) - Gyro_raw_error_z;      //TODO: check functionality
    
    /*Now we integrate the raw value in degrees per seconds in order to obtain the angle
    * If you multiply degrees/seconds by seconds you obtain degrees */
    /*---X---*/
    Gyro_angle_x = Gyro_angle_x  + Gyr_rawX*elapsedTime;
    /*---X---*/
    Gyro_angle_y = Gyro_angle_y  + Gyr_rawY*elapsedTime;
    /*---Z---*/
    Gyro_angle_z = Gyro_angle_z  + Gyr_rawZ*elapsedTime;      //TODO: check functionality

    //Serial.print("GyroX raw: ");
    //Serial.print(Gyr_rawX);
    //Serial.print("   |   ");
    //Serial.print("GyroY raw: ");
    //Serial.print(Gyr_rawY);
    //Serial.print("   |   ");
    //Serial.print("GyroX angle: ");
    //Serial.println(Gyro_angle_x);
    //Serial.print("   |   ");
    //Serial.print("GyroY angle: ");
    //Serial.println(Gyro_angle_y);
    //Serial.print("GyroZ angle: ");      //Display z values, hopefully they are alright
    //Serial.println(Gyro_angle_z);
    
    ///////////////END GYRO READ/////////////////////////
  
  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();

    // Read inputs from potentiometer and gyro
    if(incomingByte == 'I')
    {
      playerSpeed = analogRead(player);

      analogWrite(leds[0], (playerSpeed / 1023.0f) * 255);

      Serial.print(getPadded(playerSpeed));
      Serial.print("-");
      Serial.print(getPadded(getNormalisedAngle(Gyro_angle_x)));
      Serial.print("-");
      Serial.print(getPadded(getNormalisedAngle(Gyro_angle_y)));
      Serial.print("-");
      Serial.print(getPadded(getNormalisedAngle(Gyro_angle_z)));
      Serial.print("-");
      Serial.print(state);
      Serial.println();
    }
    
    if(incomingByte == 'P')
    {
      digitalWrite(leds[0], LOW);
    }
  }
}

int getNormalisedAngle(float angle)
{
  int newAngle = angle;
  newAngle = newAngle % 360;
  if(newAngle < 0)
    newAngle = abs(newAngle);
  return newAngle;
}

String getPadded(int num)
{
  char buff[5];
  char padded[6];

  sprintf(buff, "%.4u", num); // convert to 4 character string and save it to buff
  
  for(int i = 0; i < 5; i++)
    padded[i] = buff[i];
  padded[5] = '\0';

  return String(padded);

}

// Use for mapping from one range to another
float Remap(float value, float from1, float to1, float from2, float to2)
{
  return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}
