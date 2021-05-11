
/*  resistenze 220 ohm sugli anodi
 *  correnti: da misurare
*/
#define D0 16
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO 
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

#define ROSSO D1
#define VERDE D2
#define BLU D3
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

