#define FS5 740
#define D5  587
#define B5  988
#define E5  659
#define GS5 831
#define A5  880

int melody[] = {
  FS5, FS5, D5, B5, B5, E5, E5, E5, GS5, GS5, A5, B5
};

int noteDurations[] = {
  8, 8, 4, 4, 4, 4, 4, 4, 8, 8, 8, 8
};
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

void setup() 
{
  // put your setup code here, to run once:
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

    for (int thisNote = 0; thisNote < 12; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    //tone(BuzzerPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    //delay(pauseBetweenNotes);
    // stop the tone playing:
    //noTone(BuzzerPin);
  }

    if(LeftToggleSwitchState == HIGH)
    {
        //digitalWrite(LeftToggleLEDPin, HIGH);
    }else
    {
        //digitalWrite(LeftToggleLEDPin, LOW);
    }

    if(MidToggleSwitchState == HIGH)
    {
        digitalWrite(MidToggleLEDPin, HIGH);
    }else
    {
        digitalWrite(MidToggleLEDPin, LOW);
    }

    if(RightToggleSwitchState == HIGH)
    {
        //digitalWrite(RightToggleLEDPin, HIGH);
    }else
    {
        //digitalWrite(RightToggleLEDPin, LOW);
    }

    if(KeySwitchState == HIGH)
    {
        digitalWrite(KeySwitchLEDPin, HIGH);
    }else
    {
        digitalWrite(KeySwitchLEDPin, LOW);
    }
    if(MissileSwitchState < 256)
    {
      digitalWrite(MissileRedLEDPin, HIGH);
      digitalWrite(MissileGreenLEDPin, LOW);
    }else if(MissileSwitchState < 768)
    {
      digitalWrite(MissileRedLEDPin, LOW);
      digitalWrite(MissileGreenLEDPin, LOW);
    }
    else
    {
      digitalWrite(MissileRedLEDPin, LOW);
      digitalWrite(MissileGreenLEDPin, HIGH);
    }
    if(LeftTiltState > 768)
    {
      digitalWrite(LeftToggleLEDPin, HIGH);
    }else
    {
      digitalWrite(LeftToggleLEDPin, LOW);
    }
    if(RightTiltState > 768)
    {
      digitalWrite(RightToggleLEDPin, HIGH);
    }else
    {
      digitalWrite(RightToggleLEDPin, LOW);
    }
    
    digitalWrite(PotLEDPin, HIGH);
    delay(1024 - PotValue);
    digitalWrite(PotLEDPin, LOW);
    delay(1024 - PotValue);
    
}
