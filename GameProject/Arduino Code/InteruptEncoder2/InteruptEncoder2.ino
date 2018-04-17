#define encoder0PinA  2
#define encoder0PinB  3
int swtch = 7;
int swtch2 = 12;

int inByte;

volatile unsigned int encoder0Pos = 5000;
int swtchval = 0;
int swtchval2 = 0;

void setup() {
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);

  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  Serial.begin (115200);
}

void loop() {
  
  if(debounce(swtch)== HIGH)
  {
    swtchval = 1;
  }
  else
  {
    swtchval = 0;
  }

  if(debounce(swtch2)== HIGH)
  {
    swtchval2 = 1;
  }
  else
  {
    swtchval2 = 0;
  }

  
  doEncoderA();
  doEncoderB();

  
if(Serial.available() > 0) {

    inByte = Serial.read();

    if (inByte == 'V') 
    {
      
      padPrint(encoder0Pos, 4);
      Serial.print(swtchval);
      Serial.print(swtchval2);
      Serial.println();
    }
  }


  
}

void doEncoderA() {
  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == HIGH) {
      encoder0Pos = encoder0Pos + 1;          // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
  //Serial.println (encoder0Pos, DEC);
  // use for debugging - remember to comment out
}

void doEncoderB() {
  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinA) == LOW) {
      encoder0Pos = encoder0Pos + 1;          // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
}

//////////////////////////////

int debounce(int swtchPin)
{
  int state, previous_state;
  previous_state = digitalRead(swtchPin); 
  for(int i = 0; i < 25; i++)
  {
    delay(1);                       
    state = digitalRead(swtchPin);       
    if( state != previous_state)
    {
      i = 0;                  
      previous_state = state;       
    }
  }
  return state;
}

//////////////////////////////////

void padPrint( int value, int width)
{
// pads values with leading zeros to make the given width
char valueStr[6]; // large enough to hold an int
  itoa (value, valueStr, 10);
  int len = strlen(valueStr);
  if(len < width){
    len = width-len;
    while(len--)
     Serial.print('0');
  }
 Serial.print(valueStr);   
}
