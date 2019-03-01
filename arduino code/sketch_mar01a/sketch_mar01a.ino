int incomingByte;      // a variable to read incoming serial data into


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(5, OUTPUT);
}

void loop() {
   // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'P') {
      Serial.println(analogRead(A0));
    }
  }
}
