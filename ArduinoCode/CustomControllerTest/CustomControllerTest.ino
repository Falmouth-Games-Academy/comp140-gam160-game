//Template script for each compoonent Pre-Matrix
int LEDPin = 13;
int ToggleSwitchPin = 4;
int ToggleSwitchState = 0;



void setup() {
  // put your setup code here, to run once:
pinMode(LEDPin, OUTPUT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  ToggleSwitchState = digitalRead(ToggleSwitchPin);
  if(ToggleSwitchState == HIGH)
  {
    digitalWrite(LEDPin, HIGH);
  }
  else
  {
    digitalWrite(LEDPin, HIGH);
  }
 
  
}
