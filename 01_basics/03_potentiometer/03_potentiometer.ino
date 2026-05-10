const byte potent = A0; // analog pin for potentiometer connected to A0
const byte LED = 9; // LED connected to digital pin 9 (supports PWM)
int val = 0; // variable to store analog reading from 0 to 1023 (10-bit ADC)
int val1 = 0; // variable to store mapped value from 0 to 255 (8-bit PWM range)

void setup() {
  pinMode(LED, OUTPUT); // set LED pin as output
}

void loop() {
  val = analogRead(potent); // read potentiometer value
  val1 = map(val, 0, 1023, 0, 255); // map 0-1023 to 0-255
  analogWrite(LED, val1); // output PWM value to LED to control brightness
}