
/*  resistenza 100 ohm sul catodo
 *  correnti: r 15.98 g 14.23  b 11.40  w 17.58
*/
#define ROSSO 2
#define VERDE 3
#define BLU 4
#define ACCESO 1000
#define SPENTO 500

void setup() {
  // put your setup code here, to run once:
    pinMode(ROSSO, OUTPUT);
    pinMode(VERDE, OUTPUT);
    pinMode(BLU, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ROSSO, HIGH);   
  delay(ACCESO);                 
  digitalWrite(ROSSO, LOW);    
  delay(SPENTO);                 

  digitalWrite(VERDE, HIGH);   
  delay(ACCESO);                 
  digitalWrite(VERDE, LOW);    
  delay(SPENTO); 
  
  digitalWrite(BLU, HIGH);   
  delay(ACCESO);                 
  digitalWrite(BLU, LOW);    
  delay(SPENTO);

  digitalWrite(ROSSO, HIGH);
  digitalWrite(VERDE, HIGH);
  digitalWrite(BLU, HIGH);  
  delay(ACCESO);                 
  digitalWrite(ROSSO, LOW);
  digitalWrite(VERDE, LOW);
  digitalWrite(BLU, LOW);  
  delay(SPENTO); 
  
}

