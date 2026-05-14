#include <DHT.h> // library for DHT temperature and humidity sensor
#define DHTPIN 8 // DHT11 connected to digital pin 8
#define DHTTYPE DHT11 // my sensor type: DHT11
DHT dht(DHTPIN, DHTTYPE); // stores pin number and sensor type

#include <LiquidCrystal.h> // library for LCD display
const byte rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // declare all needed logic pins
// D4 D5 D6 and D7 are used to transmit 8-bit data in two steps: 2 times 4 bits (saves pins on the board, so we don't use D0-D3)
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long previous_time = 0;
const unsigned long interval = 2000;

void setup() {
  dht.begin(); // start DHT

  lcd.begin(20, 4); // start LCD
  // my LCD model is 2004, so 20 columns and 4 rows
  lcd.setCursor(0, 0); // set cursor position
  lcd.print("Temp & Humidity"); // print "Temp & Humidity"
}

void loop() {
  unsigned long current_time = millis();
  // non-blocking delay using millis, 2 seconds (according to DHT datasheet)
  if (current_time - previous_time >= interval) {
    previous_time = current_time;
    float T = dht.readTemperature(); // read temperature in Celsius
    float H = dht.readHumidity(); // read humidity in percent
    if (isnan(T) || isnan(H)) { // if we got no data for either humidity or temperature (or both), clear screen and show error
      lcd.clear(); // clear LCD
      lcd.setCursor(0, 0); // cursor to start
      lcd.print("ERROR"); // print "ERROR"
    }
    else { // if both temperature and humidity data are valid
      // lcd.setCursor(0, 0) and lcd.print("Temp & Humidity") are repeated because if an error occurred, "Temp & Humidity" would have been erased, and we need to print it again
      lcd.setCursor(0, 0); 
      lcd.print("Temp & Humidity"); 
      
      // temperature output
      lcd.setCursor(0, 1);
      lcd.print("Temp: ");
      lcd.print(T, 1); // print 1 decimal place – enough for our needs, no need to occupy extra space with second decimal
      lcd.print(" ");
      lcd.print((char)223); // degree symbol
      lcd.print("C");

      // humidity output
      lcd.setCursor(0,2);
      lcd.print("Hum: ");
      lcd.print(H, 1); // also only 1 decimal place
      lcd.print(" %");
    }
  }
}