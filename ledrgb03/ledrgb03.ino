#include <FastLED.h>
#define NUM_LEDS 4
const int dataline = 8;
CRGB leds[NUM_LEDS];


void setup()
{
  Serial.begin(9600); 
  LEDS.addLeds<WS2812B, dataline>(leds, NUM_LEDS);
  for (int p=0;p< NUM_LEDS;p++){
    leds[p] = CRGB::Black;  
    FastLED.show();}  
}

void loop() { 
  // Fade in/fade out
  for(int j = 0; j < 3; j++ ) { 
    memset(leds, 0, NUM_LEDS * 3);
    for(int k = 0; k < 256; k++) { 
      for(int i = 0; i < NUM_LEDS; i++ ) {
        switch(j) { 
          case 0: leds[i].r = k; break;
          case 1: leds[i].g = k; break;
          case 2: leds[i].b = k; break;
        }
      }
      FastLED.show();
      delay(10);
    }
    
    delay(1000);
    for(int i = 0; i < NUM_LEDS; i++ ) {
       leds[i] =  CRGB::Black;
       FastLED.show();
       delay(200);
    }
   }
  }

