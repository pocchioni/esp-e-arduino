/* http://www.bajdi.com L9110 motor driver controlling 2 small DC motors
Modificato da www.adrirobot.it */

/************************* NodeMCU pin mapping *******************************/

const int AIA = 5;  // (pwm) pin 5 connected to pin A-IA
const int AIB = 6;  // (pwm) pin 6 connected to pin A-IB
const int BIA = 10; // (pwm) pin 10 connected to pin B-IA
const int BIB = 11;  // (pwm) pin 11 connected to pin B-IB
byte speed = 100;  // change this (0-255) to control the speed of the motors

void setup() {
  pinMode(AIA, OUTPUT); // set pins to output
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);
  Serial.begin(115200);
}
void loop() {
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
