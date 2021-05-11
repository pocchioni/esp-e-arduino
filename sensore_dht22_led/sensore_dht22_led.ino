/***************************************************
  Lettura di un sensore DHT22 e invio dati in mqtt
  installare librerie Adafruit
  Sensors
  DHT
  FastLED
  
 ****************************************************/

#include <ESP8266WiFi.h>
#include "DHT.h"
#include <FastLED.h>
#include "access.h"

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

/************************* RGB led *******************************************/
#define NUM_LEDS 1
const int dataline = D2;
CRGB leds[NUM_LEDS];

/************************* Settaggi sensore *********************************/
#define DHTPIN D1     
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

/*************************** Sketch Code ************************************/
void setup() {
  Serial.begin(9600);
  LEDS.addLeds<WS2812B, dataline>(leds, NUM_LEDS);
  dht.begin();
  delay(10);
  leds[0] =  CRGB::Red;
  FastLED.show();
  Serial.println(F("Lettura sensore temperatura DHT 22"));
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());


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
  
}

