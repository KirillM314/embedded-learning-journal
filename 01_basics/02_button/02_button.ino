const byte button_pin = 7; // connect button to digital pin 7
const byte LED_pin = 9;    // connect LED to digital pin 9

void setup() {
  Serial.begin(9600);               // baud rate for serial monitor
  pinMode(button_pin, INPUT_PULLUP); // set button pin as input with internal pull-up resistor (to avoid noise)
  pinMode(LED_pin, OUTPUT);          // set LED pin as output (to supply voltage to the LED)
}

void loop() {
  boolean buttonState = !digitalRead(button_pin); // read button state (inverted because of INPUT_PULLUP)
  // boolean type is used because the button has only two states: pressed (1) or released (0)

  if (buttonState == 0) {          // if button is released
    digitalWrite(LED_pin, LOW);    // turn LED off
  } else {                         // if button is pressed
    digitalWrite(LED_pin, HIGH);   // turn LED on
  }

  Serial.println(buttonState);      // print button state to serial monitor
  delay(50);                        // small delay to stabilise serial output
}