
int potPin = 2;    // select the input pin for the potentiometer
int ledPin = 13;   // select the pin for the LED
int ValueOfPotentiometer = 0;       // variable to store the value coming from the sensor

void setup() {
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
  Serial.begin(9600); //Sets the data rate in bits per second 
}

void loop() {
  ValueOfPotentiometer = analogRead(potPin); //Reads that value of the potentiometer and sets it.
  Serial.print(ValueOfPotentiometer); //Prints the value of the variable to the com port.
  Serial.print('s'); //S used to identify the begining and end of the data it wants to read. "s117s" for example.
}
 
