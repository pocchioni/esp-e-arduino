#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "access.h"

/*  resistenze 220 ohm sugli anodi
 *  correnti: da misurare
*/

#define ROSSO D1
#define VERDE D2
#define BLU D3
#define ACCESO 300
#define SPENTO 200

const char* host = "OTA-LEDS";

int led_pin = D1;
#define N_DIMMERS 3
int dimmer_pin[] = {14, 5, 15};

void setup() {
   Serial.begin(115200);

   /* switch on led */
    pinMode(ROSSO, OUTPUT);
    pinMode(VERDE, OUTPUT);
    pinMode(BLU, OUTPUT);

   Serial.println("Booting");
   WiFi.mode(WIFI_STA);

   WiFi.begin(WLAN_SSID, WLAN_PASS);
   /* rosso on in connessione wifi */ 
   digitalWrite(ROSSO, HIGH);
    
   while (WiFi.waitForConnectResult() != WL_CONNECTED){
     WiFi.begin(WLAN_SSID, WLAN_PASS);
     Serial.println("Retrying connection...");
  }
  /* rosso off */
  digitalWrite(ROSSO, LOW);

  /* configure OTA server events */
  
  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
                        for(int i=0; i<N_DIMMERS;i++)
                          analogWrite(dimmer_pin[i], 0);
                          analogWrite(led_pin,0);
                    });

  ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
                          for (int i=0;i<30;i++)
                          {
                            analogWrite(led_pin,(i*100) % 1001);
                            delay(50);
                          }
                        });

   ArduinoOTA.onError([](ota_error_t error) { (void)error; ESP.restart(); });

   /* setup the OTA server */
   ArduinoOTA.begin();
   Serial.println("Ready");

   /* verde on */ 
   digitalWrite(VERDE, HIGH);  
   delay(2000);                 
   digitalWrite(VERDE, LOW);  

}

void loop() {
  ArduinoOTA.handle();
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
