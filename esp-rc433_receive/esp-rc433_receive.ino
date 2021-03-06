/*
  Example for receiving
  
  https://github.com/sui77/rc-switch/
  
  If you want to visualize a telegram copy the raw data and 
  paste it into http://test.sui.li/oszi/
*/

#include <RCSwitch.h>

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

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, HIGH); 
  
  mySwitch.enableReceive(D1);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  if (mySwitch.available()) {
    Serial.print(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength());
    //Serial.print(mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata());
    Serial.println(mySwitch.getReceivedProtocol());
    mySwitch.resetAvailable();
  }
  //Serial.println(".");
}
