// NodeMCU pin mapping
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
#include "FastLED.h"
#define NUM_LEDS 4

const int dataline = D8;
CRGB leds[NUM_LEDS];

void setup() {
Serial.begin(9600); 
LEDS.addLeds<WS2812B, dataline>(leds, NUM_LEDS);
       
for (int p=0;p< NUM_LEDS;p++){
leds[p] = CRGB::Black;  
FastLED.show();}
}

int r,g,b;

void loop() {
  r=0;
  g=0;
  b=0;
  while(1) {
    leds[0].r = r;
    leds[1].g = g;
    leds[2].b = b;
    leds[3].r = r;
    leds[3].g = g;
    leds[3].b = b;
    
  FastLED.show();
  r++;
  g++;
  b++;
  //if (b=0) { b=255;}
  delay(10);
  Serial.println(b);
  }
}

