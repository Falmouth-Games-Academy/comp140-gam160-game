
const int PotPin = 2;
const int LEDPin = 7 ;

int PotValue = 0;
void setup() {
  // put your setup code here, to run once:
pinMode(LEDPin, OUTPUT);
pinMode(PotPin,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 PotValue = analogRead(PotPin);

 digitalWrite(LEDPin,HIGH);
 delay(PotValue);
 digitalWrite(LEDPin,LOW);
 delay(PotValue);
 }
