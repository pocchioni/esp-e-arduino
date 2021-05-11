/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(115200);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(11);

  // Optional set pulse length.
  mySwitch.setPulseLength(000);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() {

 


  mySwitch.send(0000, 12);
  delay(3000);  
  Serial.print("Inviato ");
  Serial.println(millis());    
}
