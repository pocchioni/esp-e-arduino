/*
  Example for receiving
  
  https://github.com/sui77/rc-switch/
  
  If you want to visualize a telegram copy the raw data and 
  paste it into http://test.sui.li/oszi/
*/

#include <RCSwitch.h>
#include <SPI.h>
#include <SD.h>

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

#define LED D2 

RCSwitch mySwitch = RCSwitch();

const int chipSelect = D8;  //CS collegato alla SD


void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH); //il led è sul vcc
  mySwitch.enableReceive(D1);  // Receiver on pin D1

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    lampeggio(5);
    digitalWrite(LED, LOW);
    delay(1000);
    digitalWrite(LED, HIGH);
    return;
  }
  File dataFile = SD.open("log433.txt", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
       dataFile.println("Log telecomandi avviato...");
       dataFile.close();
       lampeggio(2);
     }
     // if the file isn't open, pop up an error:
     else {
       Serial.println("error opening log433.txt");
       lampeggio(3);
       digitalWrite(LED, LOW);
       delay(1000);
       digitalWrite(LED, HIGH);
       lampeggio(3);
       return;
     }

  Serial.println("card initialized.");

  
}

void loop() {

  
  
  if (mySwitch.available()) {
    
    //scrivo i dati sulla seriale, vedi il file output
    lampeggio(2);
    output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
    mySwitch.resetAvailable();

    // anche la SD la scrivo su output
    
    
  }
}


void lampeggio(int maxLamp){
    for (int lamp=0; lamp < maxLamp+1; lamp++) {
    digitalWrite(LED, LOW);
    delay(200);
    digitalWrite(LED, HIGH);
    delay(200);

    
   }
  }
