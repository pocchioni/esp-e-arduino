/*
  WiFi Web Server
 
 A simple web server that shows the value of the analog input pins.
 using a WiFi shield.
 
 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 Circuit:
 * WiFi shield attached
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <ESP8266WiFi.h>


char ssid[] = "";      // your network SSID (name)
char pass[] = "";   // your network password
int keyIndex = 0;        // your network key Index number (needed only for WEP)
int flash = 0;
int soglia = 200;

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  // init pin A0
  pinMode(A0, INPUT);
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  server.begin();
  // you're connected now, so print out the status:
  
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of analog input pin
            int sensorReading = analogRead(A0);
            client.print("analog input 0 is ");
            client.print(sensorReading);
            client.println("<br />");
            client.print("Numero dei flash ");
            client.print(flash);      
          
          client.println("</html>");
           break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
   
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
  /*
  // Read the photo sensor value 
  if (analogRead(A0) > soglia) { 
    while (analogRead(A0) > soglia) { 
       // Just wait the flash to turn off (to avoid multiple counts)    
    }  
  flash++; 
  Serial.print("Flash: "); 
  Serial.println(flash); 
  }
  */
}



