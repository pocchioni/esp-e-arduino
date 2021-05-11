#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

#define VT_PIN A0 
#define AT_PIN A1
RTC_DS1307 RTC;

// 10 for arduino, 53 for arduino mega
const int chipSelect = 53;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  Wire.begin();
  RTC.begin();
 
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {

  // lettura
  int vt_read = analogRead(VT_PIN);
  int at_read = analogRead(AT_PIN);

  float voltage = vt_read * (5.0 / 1024.0) * 5.0;
  float current = at_read * (5.0 / 1024.0);
  float watts = voltage * current;

  DateTime now = RTC.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

   
  Serial.print("Volts: "); 
  Serial.print(voltage, 3);
  Serial.print("\tAmps: ");
  Serial.print(current,3);
  Serial.print("\tWatts: ");
  Serial.println(watts,3);
  
  delay(5000);


  
  // make a string for assembling the data to log:
  String logString = "";
  // append to the string:
  logString = String(now.day(),DEC)+"/"+String(now.month(),DEC)+"/"+String(now.year(),DEC);
  logString += ";"+ String(now.hour(),DEC) + ":"+String(now.minute(),DEC) + ":"+String(now.second(),DEC);
  logString += ";"+String(voltage, DEC);
  // print to the serial port too:
  Serial.print("String log: ");
  Serial.println(logString);
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File logFile = SD.open("vartclog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (logFile) {
    
    logFile.println(logString);
    logFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file log");
  }
}
