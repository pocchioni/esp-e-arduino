/***************************************************
  Lettura di un sensore DHT22 e invio dati in mqtt
  installare librerie Adafruit
  Sensors
  DHT
  MQTT

  Aggiunto aggiornamento OTA

  
 ****************************************************/

#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "access.h"

/**************************** led *******************************************/
#define LED D2 

/************************* Settaggi sensore *********************************/
#define DHTPIN D1     
// comment/uncomment the right line
#define DHTTYPE DHT11
//#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);

/************************* Host per OTA *************************************/
const char* host = "OTA-CASA99";

/************************* Tempo di attesa  *********************************/
// numero di minuti tra una invio di un messaggio mqtt e un altro 
static const int TEMPO_INVIO = 1; // in minuti
// numero di letture nel tempo di invio (se il tempo è 10 e le letture sono 10 legge una volta al minuto)
static const int NUMERO_LETTURE = 5; 

/************ Global State (you don't need to change this!) *****************/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
Adafruit_MQTT_Publish esp8266_feed = Adafruit_MQTT_Publish(&mqtt, "sensors-test/casa/99");

/*************************** Sketch Code ************************************/
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  dht.begin();
  delay(10);
  
  digitalWrite(LED, HIGH);
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

  digitalWrite(LED, LOW);

  //calcolo attesa
  attesa = (int) (TEMPO_INVIO * 60000 / NUMERO_LETTURE);
  Serial.print(F("Tempo di attesa per ogni lettura in millisecondi "));
  Serial.println(attesa);

  /* configure OTA server events */
  ArduinoOTA.setHostname(host);
  ArduinoOTA.onError([](ota_error_t error) { (void)error; ESP.restart(); });
  /* setup the OTA server */
  ArduinoOTA.begin();
}

float h,t,tempH,tempT;
int lettureNonValide;
char feed[100]; 
char buf_t[10], buf_h[10];
char pos[]= "\"pos\": {\"type\": \"Point\", \"coordinates\": [9.1232, 39.2261]}";

void loop() {
  ArduinoOTA.handle();

  /* 
   *  aspetto Xs per una nuova lettura il ciclo di lettura dovrebbe durare comunque 2 secondi 
   *  temporizzazioni che servono per stabilizzare le letture del dht a 3.3v
   *  importante anche una resistenza da 5,6k invece di una da 10k
   */
   h = 0;
   t = 0;
   lettureNonValide = 0;
   for (int nl=0; nl < NUMERO_LETTURE; nl++) {
      //per migliore stabilità delle letture divido la pausa prima e dopo la lettura
      delay(attesa/2);
      digitalWrite(LED, HIGH);
      //leggo il sensore
      tempH = dht.readHumidity();
      tempT = dht.readTemperature();
      //scarto le letture non valide
      if (isnan(tempH) || isnan(tempT)) {
        Serial.print(F("Letture non valide "));
        Serial.println(lettureNonValide);
        lettureNonValide+=1;
      } else {
        h = h + tempH;
        t = t + tempT;
      } 
     //scrivo 
     Serial.print(F("Lettura DHT n "));
     Serial.print(nl);
     Serial.print(F(" Lettura temp "));
     Serial.print(tempT);
     Serial.print(F(" Accumulo temp "));
     Serial.println(t);
     digitalWrite(LED, LOW);
     //aspetto ... 
     delay(attesa/2);
    } 
  
  //calcolo la media delle letture escludendo le non valide
  h = h / (NUMERO_LETTURE - lettureNonValide);
  t = t / (NUMERO_LETTURE - lettureNonValide);
























  
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  Serial.println("MQTT connected");
  
  /* 
   *  aspetto Xs per una nuova lettura il ciclo di lettura lo faccio durare comunque 2 secondi 
   *  temporizzazioni che servono per stabilizzare le letture del dht a 3.3v
   *  importante anche una resistenza da 5,6k invece di una da 10k
   */
  delay(1000);
  
  //leggo il sensore
  h = dht.readHumidity();
  t = dht.readTemperature();

  //aspetto ... serve per evitare il nan 
  delay(2000);

  if (isnan(h) || isnan(t)) {
        Serial.println(F("Lettura non valida "));
        // Lsmpeggio led
        for (int i=0;i<7;i++) {
          digitalWrite(LED, HIGH);
          delay(500);
          digitalWrite(LED, LOW);
        }
        //aspetto 10 secondi
        delay(10000);
      } else {
        digitalWrite(LED, HIGH);
        dtostrf(t, 6, 2, buf_t);
        dtostrf(h, 6, 2, buf_h);
        sprintf(feed, "{\"ID\":99,\"temp\":%s,\"humy\":%s,\"status\":0}", buf_t, buf_h);
        Serial.println(feed);
        // Pubblico sul broker mqtt
        if (! esp8266_feed.publish(feed)) {
          Serial.println(F("MQTT Failed"));
        } else {
          Serial.println(F("MQTT OK!"));
        }
        digitalWrite(LED, LOW);
        //aspetto 3 minuti
        delay(180000);
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

