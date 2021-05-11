/***************************************************
  Lettura di un sensore DHT22 e invio dati in mqtt
  installare librerie Adafruit
  Sensors
  DHT
  MQTT

  
 ****************************************************/

#include "DHT.h"
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
// definizione degli accessi
#include "access.h"


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

/************************* Settaggi sensore *********************************/
#define DHTPIN D1     
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

/*
const char S_FEED[] PROGMEM = AIO_USERNAME "/feeds/sensor_data";
Adafruit_MQTT_Publish sensor_data = Adafruit_MQTT_Publish(&mqtt, S_FEED);
// Setup a feed called 'onoff' for subscribing to changes.
const char ONOFF_FEED[] PROGMEM = AIO_USERNAME "/feeds/onoff";
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, ONOFF_FEED);
*/

/*
   Definire le costanti per i nomi dei feed:
   <CLIENTE>/<CED>/<ZONA>
   
   esempio:
   cliente01/ced01/generale
   cliente01/ced01/armadio01

*/

#define CLIENTE "paolo"
#define CED     "casa"
#define ZONA    "1"

/*
   Numero dei feed 3

   temperature
   humidity
   flooding

*/


const char FEED[] PROGMEM =  CLIENTE "/" CED "/" ZONA;

Adafruit_MQTT_Publish esp8266_feed = Adafruit_MQTT_Publish(&mqtt, FEED);

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
//void MQTT_connect();

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(10);

  Serial.println(F("Lettura sensore temperatura e invio dati con mqtt"));
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  //mqtt.subscribe(&onoffbutton);
}

float h,t;
char feed[100]; 
char buf_t[10], buf_h[10];

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  //MQTT_connect();

  // aspetto 15s per una nuova lettura
  delay(60000);
  //leggo il sensore
  h = dht.readHumidity();
  t = dht.readTemperature();
    
  dtostrf(t, 6, 2, buf_t);
  dtostrf(h, 6, 2, buf_h);
  sprintf(feed, "{\"temperature\":%s,\"humidity\":%s,\"flooding\":\"OK\"}", buf_t, buf_h);
  
  Serial.println(feed);
  
    
 // Pubblico sul broker mqtt
  if (! esp8266_feed.publish(feed)) {
    Serial.println(F(" Failed"));
  } else {
    Serial.println(F(" OK!"));
  }

  
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
