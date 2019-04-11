int LEDG1 = 13;
int LEDG2 = 8;
int LEDG3 = 12;
int LEDO1 = 7;
int BUTTONG = 4;
//int BUTTONO = 2;

void setup() {
  Serial.begin(9600);
  pinMode(LEDG1, OUTPUT);
  pinMode(LEDG2, OUTPUT);
  pinMode(LEDG3, OUTPUT);
  pinMode(LEDO1, OUTPUT);
  pinMode(BUTTONG, INPUT);
 // pinMode(BUTTONO, INPUT);
}
void loop() {
  if(digitalRead(BUTTONG) == HIGH)
  {
  digitalWrite(LEDG1, HIGH);   
  digitalWrite(LEDG2, HIGH);
  digitalWrite(LEDG3, HIGH);
  }else
  {              
  digitalWrite(LEDG1, LOW);   
  digitalWrite(LEDG2, LOW);
  digitalWrite(LEDG3, LOW); 
  }    

 /* if(digitalRead(BUTTONO) == HIGH)
  {
  digitalWrite(LEDO1, HIGH);   
  //digitalWrite(LEDO2, HIGH);
  //digitalWrite(LEDO3, HIGH);
  }else
  {              
  digitalWrite(LEDO1, LOW);   
  //digitalWrite(LEDO2, LOW);
  //digitalWrite(LEDO3, LOW); 
  } */   
}



