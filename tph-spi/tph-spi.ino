#include <Wire.h>
#include <Sodaq_BMP085.h>
#include <Sodaq_SHT2x.h>

//TPH BMP sensor
Sodaq_BMP085 bmp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupTPH();
}
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Sending payload: TempSHT21T, TempBMP, PressureBMP, HumiditySHT21T");
  String reading = takeTPHReading();
  Serial.println(reading);
}
void setupTPH()
{
  //Initialize the wire protocol for the TPH sensors
  Wire.begin();

  //Initialize the TPH BMP sensor
  bmp.begin();
}
String takeTPHReading()
{
  //Create a String type data record in csv format
  //TempSHT21, TempBMP, PressureBMP, HumiditySHT21
  String data = String(SHT2x.GetTemperature())  + ", ";
  //BMPTemp is commented out, the data will be to long if you also send batt volt.
  data += String(bmp.readTemperature()) + ", ";
  data += String(bmp.readPressure() / 100)  + ", ";
  data += String(SHT2x.GetHumidity());

  return data;
}
