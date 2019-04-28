const int player = A0;

int playerSpeed;

const int leds[1] = {3};

int incomingByte = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

// Reset score and such
void resetGame()
{
  
}

// Update specific LED's based on incoming byte
void updateDashboard(int inByte)
{
  // switch based on inByte that determines which LED's are lit
}

// Read input from potentiometer and gyro
void getInput()
{

}

void loop() {
  // put your main code here, to run repeatedly:

  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();

    // Read inputs from potentiometer and gyro
    if(incomingByte == 'P')
    {
      playerSpeed = analogRead(player);

      analogWrite(leds[0], (playerSpeed / 1023.0f) * 255);
      
      Serial.print(getPadded(playerSpeed));
      Serial.print("\n");
    }

    // Reset game
    if(incomingByte == 'Q')
    {
      resetGame();
    }
  
    updateDashboard(incomingByte); 
  }
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
// Use for mapping from one range to another
float Remap(float value, float from1, float to1, float from2, float to2)
{
  return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}
