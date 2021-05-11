/*
   -- PrimoTest --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 2.4.3 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.7.12 or later version;
     - for iOS 1.4.7 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <ESP8266WiFi.h>

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "EspP001"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,2,0,2,0,35,0,11,13,1,
  1,0,9,7,12,12,2,31,88,0,
  65,4,44,9,9,9,66,130,12,34,
  13,10,2,26,3,3,6,56,8,22,
  2,26 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t button_1; // =1 if button pressed, else =0 
  uint8_t select_1; // =0 if select position A, =1 if position B, =2 if position C, ... 

    // output variables
  uint8_t led_1_r; // =0..255 LED Red brightness 
  int8_t level_1; // =0..100 level position 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_BUTTON_1 D4
#define tasto D3


void setup() 
{
  RemoteXY_Init (); 
  pinMode (PIN_BUTTON_1, OUTPUT);
  pinMode (tasto, INPUT);
  
  Serial.begin(115200);
  digitalWrite(PIN_BUTTON_1, 1);
  
  Serial.println("Avviando EspP001....");
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  digitalWrite(PIN_BUTTON_1, (RemoteXY.button_1==0)?LOW:HIGH);
  
  if (digitalRead(tasto))
    {
     RemoteXY.led_1_r = 255;
     Serial.println("Tasto premuto");
    }
  else
     RemoteXY.led_1_r = 0;

}
