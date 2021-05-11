/***************************************************
  Lettura di un sensore DHT11/22 
  installare librerie Adafruit
  DHT
    
 ****************************************************/

#include "DHT.h"

/************************* NodeMCU pin mapping *******************************/
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

#define LED D2 

/************************* Settaggi sensore *********************************/
#define DHTPIN D1     

// comment/uncomment the right line
#define DHTTYPE DHT11
//#define DHTTYPE DHT22  

DHT dht(DHTPIN, DHTTYPE);

/*************************** Sketch Code ************************************/
void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  dht.begin();
  delay(10);
  Serial.println(F("Lettura sensore temperatura uminità DHT 11/22"));
  
  Serial.println();

}

float h,t;
char buf_t[10], buf_h[10];
char feed[100]; 

void loop() {
  
  delay(1000);
  //leggo il sensore
  h = dht.readHumidity();
  t = dht.readTemperature();
  delay(1000);
  
  digitalWrite(LED, HIGH);
  dtostrf(t, 6, 2, buf_t);
  dtostrf(h, 6, 2, buf_h);
  sprintf(feed, "{\"temperature\":%s,\"humidity\":%s}", buf_t, buf_h);
  Serial.println(feed);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  
}

