
const int PotPin = 2;
const int LEDPin = 7;
const int LEDPin2 = 8;
const int LEDPin3 = 9;
const int LEDPin4 = 10;

int PotValue = 0;


void setup() {
  // put your setup code here, to run once:
pinMode(LEDPin, OUTPUT);
pinMode(LEDPin2, OUTPUT);
pinMode(LEDPin3, OUTPUT);
pinMode(LEDPin4, OUTPUT);
pinMode(PotPin,INPUT);
}

void Tricolour (int Green, int Red)
{
  digitalWrite(Red,HIGH);
  digitalWrite(Green, LOW);
  delay(500);
  digitalWrite(Red, LOW);
  digitalWrite(Green, HIGH); 
  delay(500);
  digitalWrite(Red, HIGH);
  digitalWrite(Green, HIGH);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
 PotValue = analogRead(PotPin);
if(PotValue < 200)
{
  digitalWrite(LEDPin, LOW);
  digitalWrite(LEDPin2,LOW);
  digitalWrite(LEDPin3, LOW);
  digitalWrite(LEDPin4, LOW);
}
else if(PotValue < 500)
{
  digitalWrite(LEDPin,HIGH);
  digitalWrite(LEDPin2,LOW);
  digitalWrite(LEDPin3,LOW);
  digitalWrite(LEDPin4, LOW);
}
else if(PotValue < 800)
{
  digitalWrite(LEDPin, HIGH);
  digitalWrite(LEDPin2, HIGH);
  digitalWrite(LEDPin3, LOW);
  digitalWrite(LEDPin4, LOW);
}
else
{
  digitalWrite(LEDPin,HIGH);
  digitalWrite(LEDPin2,HIGH);
  Tricolour(LEDPin4, LEDPin3);
}
tone(
}


