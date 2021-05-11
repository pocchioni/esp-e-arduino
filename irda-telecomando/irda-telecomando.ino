#include <IRremoteInt.h>

#include <IRremote.h> // use the library
int receiver = 11; // pin 1 of IR receiver to Arduino digital pin 11
IRrecv irrecv(receiver); // create instance of 'irrecv'
decode_results results;

void setup()
{
  Serial.begin(9600); // for serial monitor output
  irrecv.enableIRIn(); // Start the receiver
  pinMode(9, OUTPUT); // Pin 9 output
}
void loop()
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    Serial.println(results.value, HEX); // display it on serial monitor in hexadecimal
    irrecv.resume();// receive the next value
  }
   
}

