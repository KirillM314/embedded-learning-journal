#define BUT_PIN 25 // GPIO pin for the tactile button
#define LED_PIN 14 // GPIO pin for the external LED
#define POT_PIN 34 // GPIO pin for the potentiometer (Analog input)

// =========================================================================
// Class: DebouncedButton
// Encapsulates button bounce protection and toggle logic using millis()
// =========================================================================
class DebouncedButton {
  private:
    // Internal variables (Hardware configuration)
    byte _BUT_PIN;          // Stores the assigned button pin
    byte _LED_PIN;          // Stores the assigned LED pin
    int _LED_interval;      // Debounce time interval in milliseconds (e.g., 50ms)

    // Internal timing state
    unsigned long _previousTimeLED = 0; // Tracks the last time the button was sampled

    // State machine flags
    bool _butFlag = 0;     // Lock flag: prevents continuous triggering when held down
    bool _butStatus = 0;   // Current stable physical state of the button (1 = pressed)
    bool _ledFlag = 0;     // Tracks the current target state of the LED (0 = OFF, 1 = ON)

  public:
    // Class Constructor: Initializes the object with specific pins and intervals
    DebouncedButton(byte BUT_pin, byte LED_pin, int LED_interval) {
      _BUT_PIN = BUT_pin;
      _LED_PIN = LED_pin;
      _LED_interval = LED_interval; 
    }

    // Hardware Initialization Method: Configures GPIO modes (Replaces code in setup)
    void init() {
      pinMode(_BUT_PIN, INPUT_PULLUP); // Use internal pull-up (button connects pin to GND)
      pinMode(_LED_PIN, OUTPUT);       // Set LED pin as output
    }

    // Core Tick Method: Must be called continuously in loop() to process timing and logic
    void tick() {
      unsigned long currentTimeLED = millis(); // Get current uptime timestamp
      
      // Check if the debounce interval has passed since the last sample
      if (currentTimeLED - _previousTimeLED >= _LED_interval) {
        _previousTimeLED = currentTimeLED; // Reset the timestamp for the next cycle
        
        // Read physical pin. High state is active-low, so we invert it with "!"
        _butStatus = !digitalRead(_BUT_PIN); 
        
        // Condition A: Button is currently pressed AND the lock flag is lowered
        // This detects the exact moment the button goes from released to pressed (Falling Edge)
        if (_butStatus == 1 && _butFlag == 0) {
          _butFlag = 1;         // Raise the lock flag to block multiple clicks
          _ledFlag = !_ledFlag; // Toggle the state of the LED flag (ON->OFF or OFF->ON)
          digitalWrite(_LED_PIN, _ledFlag); // Apply the new state to the physical LED
        }
        
        // Condition B: Button is released AND the lock flag is still raised
        // This detects the moment the user lets go of the button (Rising Edge)
        if (_butStatus == 0 && _butFlag == 1) {
          _butFlag = 0;         // Lower the lock flag, allowing for the next press event
        }
      }
    }
};

// =========================================================================
// Class: FilteredPot
// Handles periodic analog reads and smooths data via an EMA digital filter
// =========================================================================
class FilteredPot {
  private:
    // Internal variables (Hardware and DSP configuration)
    byte _potPin;              // Stores the assigned analog pin (ADC1 channel)
    int _potInterval;          // Sampling rate interval in milliseconds
    float _k;                  // Smoothing factor for the filter (0.0 < k <= 1.0)

    // Internal timing and filtering states
    unsigned long _previousTimePot = 0; // Tracks the last time the ADC was sampled
    float _filteredPotValue;            // Accumulates the filtered value (keeps precision)

  public:
    // Class Constructor: Sets up the pin, filtering coefficient, and timing interval
    FilteredPot(byte potPin, float k, int potInterval) {
      _potPin = potPin;
      _k = k;
      _potInterval = potInterval;
    }

    // Initialization Method: Establishes the startup baseline reading to avoid jumps
    void init() {
      _filteredPotValue = analogRead(_potPin); // Seed the filter with initial position
    }

    // Event-Driven Update Method: Processes filtering and signals when a new sample is ready
    bool update() {
      unsigned long currentTime = millis(); // Get current uptime timestamp
      
      // Check if it's time to take a new analog sample
      if (currentTime - _previousTimePot >= _potInterval) { 
        _previousTimePot = currentTime; // Reset the timestamp for the next cycle
        
        int rawData = analogRead(_potPin); // Perform a 12-bit analog-to-digital read (0-4095)
        
        // Exponential Moving Average (EMA) Low-Pass Filter Formula
        // Integrates a fraction of raw data with the historical smoothed average
        _filteredPotValue = (rawData * _k) + (_filteredPotValue * (1.0 - _k)); 
        
        return true; // Signal to the main loop that a fresh, unique data point is ready
      }
      return false; // Return false if the time interval has not passed yet
    }

    // Getter Method: Safe public access to fetch the internal processed data
    int getValue() { 
      return (int)_filteredPotValue; // Cast the float to integer for clean data handling
    }
};

// Instance Creation (Object Instantiation)
DebouncedButton MyLED(BUT_PIN, LED_PIN, 50);  // Create MyLED object (Interval: 50ms)
FilteredPot MyPot(POT_PIN, 0.1, 50);          // Create MyPot object (Smoothing k: 0.1, Rate: 50ms)

void setup() {
  Serial.begin(115200); // Initialize serial data transmission at 115200 baud
  MyPot.init();         // Call startup logic for the potentiometer object
  MyLED.init();         // Call startup logic for the button/LED object
}

void loop() {
  // Event-Driven Data Logging: Prints to Serial Monitor ONLY when update() returns true
  if (MyPot.update()) {
      Serial.print("Filtered Value: ");
      Serial.println(MyPot.getValue());
  }

  // Continuously process the button scanning routine
  MyLED.tick(); 
}