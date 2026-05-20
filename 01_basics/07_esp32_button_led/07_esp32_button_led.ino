#define LED_PIN 14 // Pin for the external LED
#define BUT_PIN 25 // Pin for the tactile button
#define POT_PIN 34 // Pin for the potentiometer

// State Machine Flags
boolean Button_flag = 0; 
boolean ButtonStatus = 0; // Current physical status of the button
boolean LED_flag = 0;

// Hardware debounce and multi-click prevention timing
unsigned long ButtonPreviousTime = 0;
const int ButInterval = 50;

// Non-blocking delay for reading potentiometer data
unsigned long PotPreviousTime = 0;
const int PotInterval = 50;

// DSP Filter Variables
float filteredPotValue = 0; // Stores the smoothed analog reading
const float k = 0.1; // Filter coefficient (lower value = smoother output)

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUT_PIN, INPUT_PULLUP); // Utilizing internal pull-up resistor

  // Initialize the filter with the current baseline reading at startup
  filteredPotValue = analogRead(POT_PIN);
}

void loop() {
  // Get the current system timestamp
  unsigned long currentTime = millis();
  
  // --- Task 1: Debounced Button Processing ---
  if (currentTime - ButtonPreviousTime >= ButInterval) {
    ButtonPreviousTime = currentTime;
    ButtonStatus = !digitalRead(BUT_PIN); // Inverted signal due to INPUT_PULLUP state
    
    // Check for a single falling-edge transition (Button pressed)
    if(ButtonStatus == 1 && Button_flag == 0) {
      Button_flag = 1; // Raise the button lock flag
      LED_flag = !LED_flag; // Invert the LED state flag
      digitalWrite(LED_PIN, LED_flag); // Apply the new state to the physical LED pin
    }
    
    // Release the button lock flag once the physical button is released
    if (ButtonStatus == 0 && Button_flag == 1) {
      Button_flag = 0;
    }
  }

  // --- Task 2: Filtered Analog Read & Serial Data Logging ---
  if (currentTime - PotPreviousTime >= PotInterval) {
    PotPreviousTime = currentTime;
    int Pot_Data = analogRead(POT_PIN); // Read the raw analog voltage data
    
    // Exponential Moving Average (EMA) mathematical low-pass filter
    filteredPotValue = (Pot_Data * k) + (filteredPotValue * (1.0 - k)); 
    
    // Stream data to the Serial Monitor for analysis
    Serial.print("Raw data: ");
    Serial.print(Pot_Data);
    Serial.print(" ");
    Serial.print("Data with filter: ");
    Serial.println(filteredPotValue);
  }
}