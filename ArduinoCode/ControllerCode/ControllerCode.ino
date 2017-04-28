const int LeftToggleSwitchPin = 2;
int LeftToggleSwitchState = 0;
const int LeftToggleLEDPin = 6;

const int MidToggleSwitchPin = 3;
int MidToggleSwitchState = 0;
const int MidToggleLEDPin = 7;

const int RightToggleSwitchPin = 4;
int RightToggleSwitchState = 0;
const int RightToggleLEDPin = 8;

const int KeySwitchPin = 5;
int KeySwitchState = 0;
const int KeySwitchLEDPin = 9;

const int MissileSwitchPin = A0;
int MissileSwitchState = 0;
const int MissileGreenLEDPin = 10;
const int MissileRedLEDPin = 11;

const int PotPin = 1;
int PotValue = 0;
const int PotLEDPin = 12;

const int BuzzerPin = 13;

const int LeftTiltPin = 3;
int LeftTiltState = 0;

const int RightTiltPin = 2;
int RightTiltState = 0;

String EncodedData = "";

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);  // Setup serial library

  pinMode(LeftToggleLEDPin, OUTPUT); 

     pinMode(MidToggleLEDPin, OUTPUT);      

     pinMode(RightToggleLEDPin, OUTPUT);

     pinMode(KeySwitchLEDPin, OUTPUT);

     pinMode(MissileGreenLEDPin, OUTPUT);

     pinMode(MissileRedLEDPin, OUTPUT);

     pinMode(PotLEDPin, OUTPUT);

     pinMode(BuzzerPin, OUTPUT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
    LeftToggleSwitchState = digitalRead(LeftToggleSwitchPin);
    MidToggleSwitchState = digitalRead(MidToggleSwitchPin);
    RightToggleSwitchState = digitalRead(RightToggleSwitchPin);
    KeySwitchState = digitalRead(KeySwitchPin);
    MissileSwitchState = analogRead(MissileSwitchPin);
    PotValue = analogRead(PotPin);
    LeftTiltState = analogRead(LeftTiltPin);
    RightTiltState = analogRead(RightTiltPin);
 
    EncodedData = String(String("s") + "," + String(MidToggleSwitchState) +"," + String(RightToggleSwitchState) + "," + String(KeySwitchState) + "," + String(MissileSwitchState) + "," + String(PotValue) + ",");
 
    Serial.print(EncodedData);
    
    
}
