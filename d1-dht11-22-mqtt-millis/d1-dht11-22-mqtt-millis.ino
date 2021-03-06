/***************************************************
  Lettura di un sensore DHT11/22 e invio dati in mqtt
  Aggiunto:
    Connessione e disconnessione in un tempo dato
    Media temperature e unidità con numero di letture dato nel tempo tra una connessione e un'altra
    Invio immediato alla pressione di un bottone
    Temporizzazioni tramite millis eliminati i delay
    File: d1-dht11-22-mqtt-millis.ino
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
#define BUTTON D3

/**************************** bottone ***************************************/
#define LED D2 

/************************* Settaggi sensore *********************************/
#define DHTPIN D1     
// comment/uncomment the right line
#define DHTTYPE DHT11
//#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);

/************************* Tempo di attesa  *********************************/
// numero di minuti tra una invio di un messaggio mqtt e un altro 
static const int TEMPO_INVIO = 10; // in minuti
// numero di letture nel tempo di invio (se il tempo è 10 e le letture sono 10 legge una volta al minuto)
static const int NUMERO_LETTURE = 10; 

/************ Global State (you don't need to change this!) ******************/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
/************************* Con settaggi id e topic **************************/
static const char ID[] = "3";
Adafruit_MQTT_Publish esp8266_feed = Adafruit_MQTT_Publish(&mqtt, "sensors-test/casa/3");

/*************************** Sketch Code ************************************/
// variabile che mi serve per calcolare il delay tra una lettura e un'altra
int attesa;

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(BUTTON), handleInterrupt, FALLING);
 
  digitalWrite(LED, HIGH);
  dht.begin();
  delay(100);
  Serial.println(F("Lettura sensore temperatura e invio dati con mqtt"));
  Serial.println(F("File: d1-dht11-22-mqtt-millis.ino caricato a mar 2021"));
  //calcolo attesa
  attesa = (int) (TEMPO_INVIO * 60000 / NUMERO_LETTURE);
  Serial.print(F("Tempo di attesa per ogni lettura in millisecondi "));
  Serial.println(attesa);
  digitalWrite(LED, LOW);
  
}
void leggi_TH(float *t_letta, float *h_letta);
void MQTT_send(char *feedtosend);
float h=0, t=0, acc_h=0, acc_t=0;
long lettEff=0;  //Letture effettive
char feed[100]; 
char buf_t[10], buf_h[10];
char pos[]= "\"pos\": {\"type\": \"Point\", \"coordinates\": [9.0986, 39.2409]}";
unsigned long previousAttesa = 0;  //variabile per calcolare il tempo delle letture da accumulare
unsigned long previousInvio = 0;   //variabile per calcolare il tempo in cui effettuare l'invio

void loop() {
  
  /* 
   *  aspetto Xs per una nuova lettura il ciclo di lettura dovrebbe durare comunque 2 secondi 
   *  temporizzazioni che servono per stabilizzare le letture del dht a 3.3v
   *  importante anche una resistenza da 5,6k invece di una da 10k
   */
   
   unsigned long currentMillis = millis();
   
   //faccio una lettura senza invio
   if (currentMillis - previousAttesa >= attesa) {
     previousAttesa = currentMillis;
     //calcolo la media delle letture
     leggi_TH(&t, &h);
     acc_t = acc_t+t;
     acc_h = acc_h+h;
     // questa if l'ho inserita perchè lettEff non si azzerava al rientro dal primo ciclo (verificare il problema)
     if (lettEff > 1000){
      lettEff=0;
     }
     lettEff ++;
     Serial.print(F(" Accumulo temp "));
     Serial.print(acc_t);
     Serial.print(F(" Accumulo  Humy "));
     Serial.print(acc_h);
     Serial.print(F(" Letture effettive "));
     Serial.println(lettEff);
     Serial.print(F(" Millis "));
     Serial.println(currentMillis);
   }

   //faccio l'invio dei dati a tempo oppure se viene premuto il tasto D3 vale 1 se il tasto non è premuto
   if ((currentMillis - previousInvio >= TEMPO_INVIO * 60000) || !digitalRead(D3)) {
     if (!digitalRead(D3)){
        Serial.println("Tasto premuto");
        lampeggio(5);
     }
     previousInvio = currentMillis;

     //formo la stringa del feed
     acc_t=acc_t/lettEff;
     acc_h=acc_h/lettEff;
     dtostrf((acc_t), 6, 2, buf_t);
     dtostrf((acc_h), 6, 2, buf_h);
     
     //resetto gli accumuli
     lettEff=0;
     acc_t=0;
     acc_h=0;
     //per debug
     //Serial.print(F(" Letture effettive dopo reset "));
     //Serial.println(lettEff);
     
     //invio il feed
     sprintf(feed, "{\"ID\": %s,\"temp\": %s,\"hum\": %s, %s,\"status\": 0}", ID, buf_t, buf_h, pos);
     Serial.println("Sto inviando...");
     //Serial.println(feed);
     MQTT_send(feed);
   }
   
   // questa parte deve serve solo per debug
   //delay(100);
   //Serial.print(".");
}

void lampeggio(int maxLamp){
    for (int lamp=0; lamp < maxLamp+1; lamp++) {
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
    
   }
  }

void leggi_TH(float *t_letta, float *h_letta) {
  //leggo il sensore
  int lettureNonValide;
  
  lettureNonValide = 0;
  do {
     *t_letta = dht.readTemperature();
     *h_letta = dht.readHumidity();
     Serial.print(F("Letture non valide "));
     Serial.println(lettureNonValide);
     //scrivo valori letti
     Serial.print(F(" Lettura temp "));
     Serial.print(*t_letta);
     Serial.print(F(" Lettura Humy "));
     Serial.println(*h_letta);
     lettureNonValide+=1;
     lampeggio(1);
  }
  while (isnan(*t_letta) || isnan(*h_letta));
}

// Function to connect and reconnect to Send data.
void MQTT_send(char *feedtosend){
  // Pubblico sul broker mqtt
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  WIFI_connect();
  delay(100);
  //MQTT_connect();
  mqtt.connect();
  
  if (! esp8266_feed.publish(feedtosend)) {
    Serial.println(F("Send to MQTT Failed"));
  } else {
    Serial.println(F("Send to MQTT OK!"));
    Serial.println(feedtosend);
  }
 mqtt.disconnect();
 WiFi.disconnect();
 lampeggio(3);
 
 
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
