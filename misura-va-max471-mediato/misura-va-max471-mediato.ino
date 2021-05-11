// Paolo Occhioni
// MAX471 Power Meter con medie

#define VT_PIN A0 
#define AT_PIN A1
#define READ_INTERVAL 120 //in seconds
#define READ_TIMES 60 //times to read


void setup()
{
  Serial.begin(9600);
  
}

void loop()
{
  int ritardo = READ_INTERVAL/READ_TIMES;
  /*
  Serial.print("Ritardo:");
  Serial.println(ritardo);
  */
  
  long vt_read, vt_acc;
  long at_read, at_acc;

  vt_acc = 0;
  at_acc = 0;
  
  for (int i=0; i < READ_TIMES; i++) {
    vt_acc += analogRead(VT_PIN);
    at_acc += analogRead(AT_PIN);
    delay(ritardo*1000);

    /*
    Serial.print(i);
    Serial.print(" - ");
    Serial.print(vt_acc);
    Serial.print(" - ");
    Serial.println(at_acc);
    */
    }

  
  

  float voltage = vt_acc * (5.0 / 1024.0) * 5.0 / READ_TIMES;
  float current = at_acc * (5.0 / 1024.0) / READ_TIMES;
  float watts = voltage * current;
  Serial.print("{\"ID\":10,");
  Serial.print("\"Volts\": "); 
  Serial.print(voltage, 3);
  Serial.print(",\"Amps\": ");
  Serial.print(current,3);
  Serial.println(",\"status\": 0}");
  //Serial.print("\tWatts: ");
  //Serial.println(watts,3);
  
  delay(10);
  
}
