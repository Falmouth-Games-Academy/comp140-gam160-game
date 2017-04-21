/*
  Button

 Turns on and off a light emitting diode(LED) connected to digital
 pin 13, when pressing a pushbutton attached to pin 2.


 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground

 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.


 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to
// set pin numbers:

// The Input pins
const int GreenWire = 7;      // the number of the LED pin
const int RedWire =  6;
const int WhiteWire =  5;
const int OrangeWire =  4;
const int YellowWire =  3;
const int BlueWire =  2;

// The Output pins
const int YellowWireOutput = A0;
const int RedWireOutput = A1;
const int WhiteWireOutput = A2;
const int GreenWireOutput = A3;
const int BlueWireOutput = A4;
const int OrangeWireOutput = A5;
// variables will change:
int GreenState = 0;         // variables for reading the pushbutton status
int RedState = 0;  
int WhiteState = 0;  
int OrangeState = 0;
int YellowState = 0;
int BlueState = 0;

int GreenOutState = 0;    // variables that read the analog state anything above 20 is wires touching, 0 is nothing touching and 0-20 is body conduction.
int RedOutState = 0;
int WhiteOutState = 0;
int OrangeOutState = 0;
int YellowOutState = 0;
int BlueOutState = 0;

int InputSent = 0;    //The Input wires that are touching 1 = Green, 2 = Red, 3 = White, 4 = Orange, 5 = Yellow, 6 = Blue, 0 = none are touching
int OutputSent = 0;  //The Output wires that are touching 1 = Green, 2 = Red, 3 = White, 4 = Orange, 5 = Yellow, 6 = Blue, 0 = none are touching

void setup() {
  // initialize the LED pin as an output:
  Serial.begin(9600);
  //INPUT Pins
  pinMode(GreenWire, INPUT_PULLUP);
  pinMode(RedWire, INPUT_PULLUP);
  pinMode(WhiteWire, INPUT_PULLUP);
  pinMode(OrangeWire, INPUT_PULLUP);
  pinMode(YellowWire, INPUT_PULLUP);
  pinMode(BlueWire, INPUT_PULLUP);

    
}

void loop() {
  // read the state of the Input wires:
  GreenState = digitalRead(GreenWire);
  RedState = digitalRead(RedWire); 
  WhiteState = digitalRead(WhiteWire);  
  OrangeState = digitalRead(OrangeWire);
  YellowState = digitalRead(YellowWire);
  BlueState = digitalRead(BlueWire);

  // read the state of the Output wires:
  GreenOutState = analogRead(GreenWireOutput);
  RedOutState = analogRead(RedWireOutput);
  WhiteOutState = analogRead(WhiteWireOutput);
  OrangeOutState = analogRead(OrangeWireOutput);
  YellowOutState = analogRead(YellowWireOutput);
  BlueOutState = analogRead(BlueWireOutput);
  
  // Turn the states into data corresponding to each color so I can send it. InputSent and OutputSent
//INPUT SENT
  if (GreenState == 0)
{
 InputSent = 1;
}
  else if (RedState == 0)
{
 InputSent = 2;
}
  else if (WhiteState == 0)
{
 InputSent = 3;
}
  else if (OrangeState == 0)
{
 InputSent = 4;
}
  else if (YellowState == 0)
{
 InputSent = 5;
}
  else if (BlueState == 0)
{
 InputSent = 6;
}
  else 
{
 InputSent = 0;  
}

//OUTPUT SENT
  if (GreenOutState > 20) //20 To eliminate any body conduction if you touch the INPUT PIN wire with your other hand while touching the OUTPUT pin wire the analog pin reads a value of around 6.
{
  OutputSent = 1;
}
  else if (RedOutState > 20)
{
  OutputSent = 2;
}
  else if (WhiteOutState > 20)
{
  OutputSent = 3;
}
  else if (OrangeOutState > 20)
{
  OutputSent = 4;
}
  else if (YellowOutState > 20)
{
  OutputSent = 5;
}
  else if (BlueOutState > 20)
{
  OutputSent = 6;
}
else 
{
  OutputSent = 0;
}
  String SendString = String(InputSent) +","+ String(OutputSent);


  Serial.println(SendString); 
  delay(100); //Bug fixing UE4 for some reason cuts off the string or sends an uncompleted string, changing this delay changes how frequent that happens.
}
