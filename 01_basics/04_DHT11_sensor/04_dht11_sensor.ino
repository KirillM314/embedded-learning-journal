#include <DHT.h> // include the library for the DHT11 temperature and humidity sensor

#define DHTPIN 2      // sensor data pin connected to digital pin 2
#define DHTTYPE DHT11 // sensor type is DHT11

DHT dht(DHTPIN, DHTTYPE); // initialise the sensor

unsigned long previous_time = 0; // needed for timing with millis()
const int interval = 2000;       // according to the datasheet, wait 2 seconds between readings

void setup() {
  Serial.begin(9600); // standard baud rate for serial communication
  dht.begin();        // configure sensor pins and pull-up timing
}

void loop() {
  unsigned long current_time = millis(); // store the current millis counter value

  if (current_time - previous_time >= interval) { // if a 2-second interval has passed
    previous_time = current_time; // reset the timer for the next interval

    if (dht.read()) { // read data from the sensor
      Serial.print("Temperature: ");
      Serial.print(dht.readTemperature()); // output the temperature value
      Serial.print("°C, Humidity: ");
      Serial.print(dht.readHumidity()); // output the humidity value
      Serial.println("%"); // new line
    }
    else {
      Serial.println("ERROR"); // if no data was received from the sensor
    }
  }
}