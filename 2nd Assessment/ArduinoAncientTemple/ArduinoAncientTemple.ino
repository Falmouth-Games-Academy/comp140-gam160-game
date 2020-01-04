int LEDG1 = 13;
int LEDO1 = 12;
int LEDR1 = 8;
int BUTTONG = 4;
int BUTTONO = 2;
int BUTTONR = 7;
const int flexPinL = A0;
const int flexPinR = A1;
const int JumpSen = A2;

int pressed = 0;

int incomingByte;

void setup() {
  Serial.begin(9600);
  pinMode(LEDG1, OUTPUT);
  pinMode(LEDR1, OUTPUT);
  pinMode(LEDO1, OUTPUT);
  pinMode(BUTTONG, INPUT);
  pinMode(BUTTONR, INPUT);
  pinMode(BUTTONO, INPUT);
}
void loop() 
{
  SendPositions();
  if(Serial.available() > 0)
  {
  incomingByte = Serial.read();

  if(incomingByte =='F')
  {
    SendPositions();
  }


    if(digitalRead(BUTTONG) == HIGH)
    {
    digitalWrite(LEDG1, HIGH);   
    }else
    {              
    digitalWrite(LEDG1, LOW);   
    }
  
    if(digitalRead(BUTTONO) == HIGH)
    {
    digitalWrite(LEDO1, HIGH);   
    }else
    {              
    digitalWrite(LEDO1, LOW);
    }    
  
    if(digitalRead(BUTTONR) == HIGH)
    {
    digitalWrite(LEDR1, HIGH);   
    }else
    {              
    digitalWrite(LEDR1, LOW);
    }
  }
}

void SendPositions()
{
   int positionL = analogRead(flexPinL) - 723;
   int positionR = analogRead(flexPinR) - 750;
   int positionJ = analogRead(JumpSen);
  
   if(digitalRead(BUTTONG) > 0)
   pressed = 1;
   else if(digitalRead(BUTTONO) > 0)
   pressed = 2;
   else if(digitalRead(BUTTONR) > 0)
   pressed = 3;

   if (positionL < 0 )
   {
    positionL = 0;
   }

   if (positionR < 0 )
   {
    positionR = 0;
   }

  Serial.print(getPadded(positionL));
  Serial.print('-');
  Serial.print(getPadded(positionR));
  Serial.print('-');
  Serial.print(getPadded(positionJ));
  Serial.print('-');
  Serial.print(getPadded(digitalRead(BUTTONG)));
  Serial.print('-');
  Serial.print(getPadded(digitalRead(BUTTONO)));
  Serial.print('-');
  Serial.println(getPadded(digitalRead(BUTTONR)));
}

String getPadded(int num)
{
  char buff[5];
  char padded[6];

  sprintf(buff, "%.4u", num); // convert to 4 character string and save it to buff
  
  for(int i = 0; i < 5; i++)
    padded[i] = buff[i];
  padded[5] = '\0';

  return String(padded);

}



