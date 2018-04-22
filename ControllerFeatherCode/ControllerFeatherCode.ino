#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

int inByte;

//button vars
const int button1 = 2;
int button1Active;
int button2Active;
const int button2 = 14;

int buttonState1;
int lastButtonState1 = LOW;

int buttonState2;
int lastButtonState2 = LOW;

int previousState1 = 1;
int previousState2 = 1;
//end of buttons vars

void setup(void) 
{
#ifndef ESP8266
  while (!Serial);
#endif
  Serial.begin(115200);
  Serial.println("Accelerometer Test"); Serial.println("");

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);
  // displaySetRange(ADXL345_RANGE_8_G);
  // displaySetRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_2_G);

  Serial.println("");
}

void loop(void) 
{
  /* Get a new sensor event */
  sensors_event_t event; 
  accel.getEvent(&event);

  if(Serial.available() > 0)
  {
    inByte = Serial.read();
    
    if (inByte == 'P')
    {
      //Display the results (acceleration is measured in m/s^2)
      Serial.print(event.acceleration.x); Serial.print(";");
      Serial.println(event.acceleration.y);// Serial.println(" ");
      //Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
      //Serial.println("m/s^2 ");
    }

        //buttonCode
    if (inByte == 'B')
    {
        Serial.print(button1Active);
        Serial.print('-');
        Serial.println(button2Active);
    }
    
    int button1State = digitalRead(button1);
    
    if (button1State == 0 && button1State != previousState1) 
    {
      button1Active = 0;
    }else
    {
      button1Active = 1;
    }
    
    previousState1 = button1State;

        int button2State = digitalRead(button2);
    
    if (button2State == 0 && button2State != previousState2) 
    {
      button2Active = 0;
    }else
    {
      button2Active = 1;
    }
    
    previousState2 = button2State;
    //end of buttonCode
  }
}

