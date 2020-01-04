const int playerOne = A0;

const int playerTwo = A1;
//These two tell the arduino the port that the players is connected to

int Light = 3;
//This tells the arduino the port that the light is connected to

int incomingByte;


void setup() 
{

Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();

    if (incomingByte == 'P')
      {
          sendPositions();
      }

      //This tells the arduino to send the position of the players every frame
      
     if(incomingByte == 'L')
      {
        digitalWrite(Light, HIGH);
        delay(100);
        digitalWrite(Light, LOW);
        delay(100);
      }
      //This tells to flash the light on and off for the set amount of time
     
  }
}

void sendPositions()
{
  int position1 = analogRead(playerOne);
  int position2 = analogRead(playerTwo);

  Serial.print(position1);
  Serial.print('-');
  Serial.println(position2);
}

