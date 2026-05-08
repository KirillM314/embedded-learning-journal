const byte led_pin = 9; // add digital pin 9, to which the LED will be connected
// I used a constant instead of a variable because the pin connected to the LED won't change, it stays digital pin 9
// I used the byte type because pin numbers are positive integers and because it uses less memory than int (byte is 1 byte, int is 2 bytes)

void setup() // everything in void setup() runs once when Arduino starts
{
  pinMode(led_pin, OUTPUT); // set digital pin 9 as OUTPUT because we need to supply voltage to light up the LED
}

void loop() // main code: runs cyclically, top to bottom (this alternates between HIGH and LOW states, making the LED blink)
{
  digitalWrite(led_pin, HIGH); // apply voltage to digital pin 9
  delay(1000); // wait 1 second
  digitalWrite(led_pin, LOW); // remove voltage from digital pin 9
  delay(1000); // wait 1 second
}