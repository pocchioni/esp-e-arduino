 /***************************************************
 Test di un servo SG90 collegato direttamente ad arduino   
 ****************************************************/
#include <Servo.h>
Servo servo;
int angle = 0;
void setup() {
  Serial.begin(9600);
  Serial.println(F("Test di un servo SG90 collegato direttamente ad arduino"));

  servo.attach(8);
  servo.write(angle);
}
void loop() 
{ 
 // scan from 0 to 180 degrees
  Serial.println(F("Sale"));
  for(angle = 0; angle < 180; angle++) {                                  
    servo.write(angle);               
    delay(15);                   
  } 
  // now scan back from 180 to 0 degrees
  Serial.println(F("Scende"));
  for(angle = 180; angle > 0; angle--) {                                
    servo.write(angle);    
    delay(15);       
  } 

  Serial.println(F("Passaggi veloci"));
  for (int i=0; i<10; i++){
    servo.write(0);
    delay(500);
    servo.write(90);
    delay(500);
    servo.write(180);
    delay(500);
  }
}
