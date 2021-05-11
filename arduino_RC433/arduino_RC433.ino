/*
  Example for receiving
  
  https://github.com/sui77/rc-switch/
  
  If you want to visualize a telegram copy the raw data and 
  paste it into http://test.sui.li/oszi/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH); 
  
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  if (mySwitch.available()) {
    Serial.print(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength());
    Serial.print(mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata());
    Serial.println(mySwitch.getReceivedProtocol());
    mySwitch.resetAvailable();
  }
  Serial.println(".");
}
