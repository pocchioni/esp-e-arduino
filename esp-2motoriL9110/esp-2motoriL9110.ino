/* http://www.bajdi.com L9110 motor driver controlling 2 small DC motors
Modificato da www.adrirobot.it */

/************************* NodeMCU pin mapping *******************************/
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

const int AIA = D3;  // (pwm) pin 5 connected to pin A-IA
const int AIB = D4;  // (pwm) pin 6 connected to pin A-IB
const int BIA = D1; // (pwm) pin 10 connected to pin B-IA
const int BIB = D2;  // (pwm) pin 11 connected to pin B-IB
byte speed = 1000;  // change this (0-255) to control the speed of the motors

void setup() {
  pinMode(AIA, OUTPUT); // set pins to output
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);
  Serial.begin(115200);
}
void loop() {
  //test con led
    // increase the LED brightness
  Serial.println( "Test D1 con led aumenta..." );
  for(int dutyCycle = 0; dutyCycle < 1023; dutyCycle++){   
    // changing the LED brightness with PWM
    analogWrite(BIA, dutyCycle);
    delay(10);
  }

  // decrease the LED brightness
  Serial.println( "Test D1 con led diminuisce..." );
  for(int dutyCycle = 1023; dutyCycle > 0; dutyCycle--){
    // changing the LED brightness with PWM
    analogWrite(BIA, dutyCycle);
    delay(10);
  }





  
  Serial.println( "Avanti..." );
  forward();
  delay(2000);
  STOP();
  Serial.println( "Indietro..." );
  backward();
  delay(2000);
  STOP();
  Serial.println( "Sinistra..." );
  left();
  delay(2000);
  STOP();
  Serial.println( "Destra..." );
  right();
  delay(2000);
  STOP();
}
void backward()
{
  analogWrite(AIA, 0);
  analogWrite(AIB, speed);
  analogWrite(BIA, 0);
  analogWrite(BIB, speed);
}
void forward()
{
  analogWrite(AIA, speed);
  analogWrite(AIB, 0);
  analogWrite(BIA, speed);
  analogWrite(BIB, 0);
}
void left()
{
  analogWrite(AIA, speed);
  analogWrite(AIB, 0);
  analogWrite(BIA, 0);
  analogWrite(BIB, speed);
}
void right()
{
  analogWrite(AIA, 0);
  analogWrite(AIB, speed);
  analogWrite(BIA, speed);
  analogWrite(BIB, 0);
}
void STOP()
{
  analogWrite(AIA, 0);
  analogWrite(AIB, 0);
  analogWrite(BIA, 0);
  analogWrite(BIB, 0);
  delay (2000);
}
