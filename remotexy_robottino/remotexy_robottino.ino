/*
   -- Robottino --

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
#define REMOTEXY_WIFI_SSID "Robottino001"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6377


#define PIN_OCCHI D4

#define PIN_BRACCIOSX D1
#define PIN_BRACCIODX D2
#define PIN_TESTA D3

// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
{ 255, 4, 0, 0, 0, 54, 0, 11, 176, 0,
  4, 32, 5, 18, 6, 33, 2, 26, 4, 32,
  89, 17, 6, 33, 2, 26, 4, 160, 27, 14,
  45, 7, 2, 26, 1, 0, 43, 32, 12, 12,
  1, 31, 79, 0, 129, 0, 34, 3, 29, 6,
  17, 82, 111, 98, 111, 116, 116, 105, 110, 111,
  0
};

// this structure defines all the variables and events of your control interface
struct {

  // input variables
  int8_t braccio_sx; // =-100..100 slider position
  int8_t braccio_dx; // =-100..100 slider position
  int8_t testa; // =-100..100 slider position
  uint8_t occhi; // =1 if button pressed, else =0

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#include <Servo.h>

Servo left_servo;
Servo right_servo;
Servo head_servo;

int valdx, valsx, valt; 
unsigned char occhi_state = 0;
unsigned long occhi_time = 0;
unsigned long prev_time = 0;



void setup()
{
  RemoteXY_Init ();


  Serial.begin(115200);
  Serial.println("Avviando Robottino....");


  pinMode (PIN_OCCHI, OUTPUT);

  left_servo.attach(PIN_BRACCIOSX);
  right_servo.attach(PIN_BRACCIODX);
  head_servo.attach(PIN_TESTA);

  RemoteXY.braccio_sx = 50;
  RemoteXY.braccio_dx = 50;
  RemoteXY.testa = 50;


}

void loop()
{
  unsigned long time = millis();
  unsigned long d_time = time - prev_time;
  prev_time = time;
  
  RemoteXY_Handler ();

  /*
   
  
  left_servo.writeMicroseconds(RemoteXY.braccio_sx*20+500); 
  right_servo.writeMicroseconds(2500-RemoteXY.braccio_dx*20); 
  head_servo.writeMicroseconds(RemoteXY.testa*20+500); 
   */
  valsx = map(RemoteXY.braccio_sx, -100, 100, 180, 0);
  valdx = map(RemoteXY.braccio_dx, -100, 100, 0, 180);
  valt = map(RemoteXY.testa, -100, 100, 0, 180);
  /*
  Serial.print("SX: ");
  Serial.print(RemoteXY.braccio_sx);
  Serial.print(" map: ");
  Serial.println(valsx);
  Serial.print("DX: ");
  Serial.print(RemoteXY.braccio_dx);
  Serial.print(" map: ");
  Serial.println(valdx);
  */

  left_servo.write(valsx); 
  right_servo.write(valdx); 
  head_servo.write(valt);
  
}
