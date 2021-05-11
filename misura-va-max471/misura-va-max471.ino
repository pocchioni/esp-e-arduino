// Henry's Bench
// MAX471 Power Meter Tutorial

#define VT_PIN A0 
#define AT_PIN A1

void setup()
{
  Serial.begin(9600);
  
}

void loop()
{
  int vt_read = analogRead(VT_PIN);
  int at_read = analogRead(AT_PIN);

  float voltage = vt_read * (5.0 / 1024.0) * 5.0;
  float current = at_read * (5.0 / 1024.0);
  float watts = voltage * current;
  
  Serial.print("Volts: "); 
  Serial.print(voltage, 3);
  Serial.print("\tAmps: ");
  Serial.print(current,3);
  Serial.print("\tWatts: ");
  Serial.println(watts,3);
  
  delay(500);
  
}
