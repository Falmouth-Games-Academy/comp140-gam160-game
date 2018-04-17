int val;
int encoder0PinA = 3;
int encoder0PinB = 4;
int encoder0Pos = 5000;
int encoder0PinALast = LOW;
int rot0 = LOW;
/*
int encoder1PinA = 3;
int encoder1PinB = 4;
int encoder1Pos = 5000;
int encoder1PinALast = LOW;
int rot1 = LOW;
*/
int swtch = 7;
int swtchval = 0;

int inByte;

void setup() {
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);

  //pinMode (encoder1PinA, INPUT);
  //pinMode (encoder1PinB, INPUT);

  pinMode(swtch, INPUT);
  
  Serial.begin (9600);
}

void loop() {
  rot0 = digitalRead(encoder0PinA);
  
//  rot1 = digitalRead(encoder1PinA);

  //swtch = digitalRead(swtch0PinA);
  
  if ((encoder0PinALast == LOW) && (rot0 == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      encoder0Pos++;
    }
  }
  padPrint(encoder0Pos, 4);
 
  Serial.print(swtchval);
  Serial.println();
  encoder0PinALast = rot0;
  

  

//////////////////
/*
  if(debounce(swtch)== HIGH)
  {
    swtchval = 1;
  }
  else
  {
    swtchval = 0;
  }
  

//////////////////
*/

  /*
  if(Serial.available() > 0) {

    inByte = Serial.read();

    if (inByte == 'V') 
    {
      padPrint(encoder0Pos, 4);
      padPrint(encoder1Pos, 4);
      Serial.print(swtchval);
      Serial.println();
    }
  }*/
  
}

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
/*
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
}*/

