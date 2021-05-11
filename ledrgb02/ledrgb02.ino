#include "FastLED.h"
#define NUM_LEDS 4
const int dataline = 8;
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

