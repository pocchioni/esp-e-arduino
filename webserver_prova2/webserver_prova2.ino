#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//Controllare se è presente #include<ESP8266WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>

/* Variabili per la connessione alla WIFI: utente, password e porta */
char ssid[] = "user";
char pass[] = "pass";
//ESP8266WebServer server(80);
WiFiServer server(80);

/* Variabili per il calcolo dei flash */
int soglia = 450;
int flash = 0;
int durata_flash;


void setup() {
  pinMode(A0, INPUT);
  Serial.begin(115200);   /* Connessione seriale all'ESP (dovrebbe andare con 115200) */

  /* Connessione alla WIFI */
  WiFi.begin(ssid, pass);
  Serial.print("\n\r \n\rWorking to connect");

  /* Attesa di connessione */
  while (WiFi.status() != WL_CONNECTED) {
    /* Finchè aspetto stampo questa stringa su Seriale */
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    /* Connessione alla rete WPA/WPA2 con i valori impostati prima utente e password */ 
    WiFi.begin(ssid, pass);

    /* Aspetto 10 (10000 millisecondi) secondi per la connessione (oppure 5) */
    delay(5000);
  }
  /* Stampo l'SSID della connessione a cui ci siamo connessi */
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  /* Stampa, sempre su seriale l'ip locale */
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  /* Avvio del server sulla WIFI a cui ci siamo connessi */
  server.begin();

  
  /* Noi siamo connessi ora, quindi stampiamo lo status */
  
}

void loop() {


  WiFiClient client = server.available();
  if(client) {

    Serial.println("Trovato nuovo client");

     while (client.connected()) {
      if (client.available()) {

        char c = client.read();
        Serial.write(c);
        
        /* Calcolo dei flash */
        // Read the photo sensor value 
        if (analogRead(A0) < soglia) 
        { 
          durata_flash=0;
          while (analogRead(A0) < soglia) 
          { 
            // calcola la durara del flash
            durata_flash++;
          }  

          flash++; 
          Serial.print("Flash ");
          Serial.print(flash);
          Serial.print(" durata flash ");
          Serial.println(durata_flash);
        }
        delay(10);
        
        /* Qui segue la composizione della pagina web sul browser */
        /* Qui segue la composizione della pagina web sul browser */
   /* Meta-Dati */
   client.println("HTTP/1.1 200 OK");
   client.println("Content-Type: text/html");
   client.println("Connection: close");  // the connection will be closed after completion of the response
          
   client.println();
   client.println("<!DOCTYPE HTML>");
   client.println("<html>");
   /* Inizio HTML */
   int sensorReading = analogRead(A0);
   client.print("Analog input 0 is ");
   client.print(sensorReading);
   client.println("<br />");
            
   /* Stampa del numero dei flash */
   client.print("Numero dei flash ");
   client.print(flash);      
          
   client.println("</html>");
        
        break;

      } /* Fine if interno */
     } /* Fine while */
   } /* Fine if esterno */

 } /* Fine loop */


 
