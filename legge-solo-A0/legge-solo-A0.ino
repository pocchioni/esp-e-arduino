
int soglia = 450;
int flash = 0;
int durata_flash;
void setup() {
 
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  /*
  int sensorReading = analogRead(A0);
  Serial.print("analog input 0 is ");
  Serial.println(sensorReading);
  delay(100);
  */

  // Read the photo sensor value 
  if (analogRead(A0) < soglia) 
  { 
     durata_flash=0;
     while (analogRead(A0) < soglia) 
     { 
      // calcola la durara del flash
      durata_flash++;
      delay(1);
     }  

     flash++; 
     Serial.print("Flash ");
     Serial.print(flash);
     Serial.print(" durata flash ");
     Serial.println(durata_flash);
  }
  delay(10);
} 



