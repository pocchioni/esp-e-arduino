/***************************************************
  Lettura di un sensore DHT22 e invio dati in mqtt
  installare librerie Adafruit
  Sensors
  DHT
  FastLED
  
 ****************************************************/

#include "DHT.h"
#include <FastLED.h>


/************************* RGB led *******************************************/
#define NUM_LEDS 1
const int dataline = 2;
CRGB leds[NUM_LEDS];

/************************* Settaggi sensore *********************************/
#define DHTPIN 3     
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

/*************************** Sketch Code ************************************/

void setup() {
  Serial.begin(9600);
  LEDS.addLeds<WS2812B, dataline>(leds, NUM_LEDS);
  dht.begin();
  delay(10);
  leds[0] =  CRGB::Red;
  FastLED.show();  
  Serial.println(F("Lettura sensore temperatura DHT 22"));
  
  Serial.println();

  Serial.println("Start");
  delay(5000);
  leds[0] =  CRGB::Green;
  FastLED.show();
}

float h,t;
char buf_t[10], buf_h[10];
char feed[100]; 

void loop() {
  
  delay(1000);
  //leggo il sensore
  leds[0] =  CRGB::Blue;
  FastLED.show();
  h = dht.readHumidity();
  t = dht.readTemperature();
  leds[0] =  CRGB::Green;
  FastLED.show();
  
  dtostrf(t, 6, 2, buf_t);
  dtostrf(h, 6, 2, buf_h);
  sprintf(feed, "{\"temperature\":%s,\"humidity\":%s}", buf_t, buf_h);
    Serial.println(feed);
  
  leds[0] =  CRGB::Black;
  FastLED.show();
  
}

