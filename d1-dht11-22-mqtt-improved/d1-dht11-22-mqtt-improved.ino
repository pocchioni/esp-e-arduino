/***************************************************
  Lettura di un sensore DHT22 e invio dati in mqtt
  Aggiunto:
    Connessione e disconnessione in un tempo dato
    Media temperature e unidità con numero di letture dato nel tempo tra una connessione e un'altra
  
  installare librerie Adafruit
  Sensors
  DHT
  MQTT

  
 ****************************************************/

#include "DHT.h"
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "access.h"

/************************* NodeMCU pin mapping *******************************/
#define D0 16
#define D1 5 // I2C Bus SCL (clock) GPIO5
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO 
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

/**************************** led *******************************************/
#define LED D2 

/************************* Settaggi sensore *********************************/
#define DHTPIN D1     
// comment/uncomment the right line
#define DHTTYPE DHT11
//#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);

/************************* Tempo di attesa  *********************************/
// numero di minuti tra una invio di un messaggio mqtt e un altro 
static const int TEMPO_INVIO = 1; // in minuti
// numero di letture nel tempo di invio (se il tempo è 10 e le letture sono 10 legge una volta al minuto)
static const int NUMERO_LETTURE = 5; 

/************************* Settaggi id e topic *******************************/
static const char ID[] = "5";

/************ Global State (you don't need to change this!) ******************/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
Adafruit_MQTT_Publish esp8266_feed = Adafruit_MQTT_Publish(&mqtt, "sensors-test/casa/5");

/*************************** Sketch Code ************************************/
// variabile che mi serve per calcolare il delay tra una lettura e un'altra
int attesa;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  dht.begin();
  delay(10);
  digitalWrite(LED, HIGH);
  Serial.println(F("Lettura sensore temperatura e invio dati con mqtt"));
  digitalWrite(LED, LOW);

  //calcolo attesa
  attesa = (int) (TEMPO_INVIO * 60000 / NUMERO_LETTURE);
  Serial.print(F("Tempo di attesa per ogni lettura in millisecondi "));
  Serial.println(attesa);
  
}
float h,t,tempH,tempT;
int lettureNonValide;
char feed[100]; 
char buf_t[10], buf_h[10];
char pos[]= "\"pos\": {\"type\": \"Point\", \"coordinates\": [9.0986, 39.2409]}";


void loop() {
  
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

  
  digitalWrite(LED, HIGH);
  
  //formo la stringa del feed
  dtostrf(t, 6, 2, buf_t);
  dtostrf(h, 6, 2, buf_h);
  sprintf(feed, "{\"ID\": %s,\"temp\": %s,\"hum\": %s, %s,\"status\": 0}", ID, buf_t, buf_h, pos);
  Serial.println(feed);
  
  // Pubblico sul broker mqtt
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  WIFI_connect();
  delay(500);
  //MQTT_connect();
  mqtt.connect();
  
  if (! esp8266_feed.publish(feed)) {
    Serial.println(F("MQTT Failed"));
  } else {
    Serial.println(F("MQTT OK!"));
    Serial.println(feed);
  }
 mqtt.disconnect();
 WiFi.disconnect();
 digitalWrite(LED, LOW);
 
}

// Function to connect and reconnect to WiFi.
void WIFI_connect() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  
  WiFi.persistent(false);
  //WiFi.mode(WIFI_OFF);   // this is a temporary line, to be removed after SDK update to 1.5.4
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  
  
  //WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
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

  uint8_t retries = 5;
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

