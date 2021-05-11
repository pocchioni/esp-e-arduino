/***************************************************
  Lettura di un sensore DHT11 e invio dati in mqtt
 ****************************************************/

#include "DHT.h"
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "ssid"
#define WLAN_PASS       "pass"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "server"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "user"
#define AIO_KEY         "pass"

/************************* Settaggi sensore *********************************/
#define DHTPIN 2     // ATTENZIONE nella nodeMcu la mappatura dei pin è diversa quindi  pin2 = pin4
#define DHTTYPE DHT11   // DHT 11 
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

#define CLIENTE "customer01"
#define CED     "dc02"
#define ZONA    "general"

/*
   Numero dei feed 3

   temperature
   humidity
   flooding

*/


const char T_FEED[] PROGMEM =  CLIENTE "/" CED "/" ZONA;
const char H_FEED[] PROGMEM =  CLIENTE "/" CED "/" ZONA;
const char F_FEED[] PROGMEM =  CLIENTE "/" CED "/" ZONA;

Adafruit_MQTT_Publish esp8266_temp = Adafruit_MQTT_Publish(&mqtt, T_FEED);
Adafruit_MQTT_Publish esp8266_humy = Adafruit_MQTT_Publish(&mqtt, H_FEED);
Adafruit_MQTT_Publish esp8266_floo = Adafruit_MQTT_Publish(&mqtt, F_FEED);



/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

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
char feed_t[20], feed_h[20], feed_f[20]; 
char buf[10];

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

 
  // aspetto 15s per una nuova lettura
  delay(15000);
  //leggo il sensore
  h = dht.readHumidity();
  t = dht.readTemperature();
     
  /*
   // Test
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(" ");
  */
  
  // Pubblico la temp sul broker mqtt

  dtostrf(t, 6, 2, buf);
  sprintf(feed_t, "{\"temperature\":%s}",buf);
  dtostrf(h, 6, 2, buf);
  sprintf(feed_h, "{\"humidity\":%s}",buf);
  sprintf(feed_f, "{\"flooding\":OK}");
  
  Serial.println(feed_t);
  Serial.println(feed_h);
  Serial.println(feed_f);
  
 // Pubblico sul broker mqtt
  if (! esp8266_temp.publish(feed_t)) {
    Serial.println(F(" T Failed"));
  } else {
    Serial.println(F(" T OK!"));
  }

  if (! esp8266_humy.publish(feed_h)) {
    Serial.println(F(" H Failed"));
  } else {
    Serial.println(F(" H OK!"));
  }

  if (! esp8266_humy.publish(feed_f)) {
    Serial.println(F(" F Failed"));
  } else {
    Serial.println(F(" F OK!"));
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
