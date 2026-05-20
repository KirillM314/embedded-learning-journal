# Project 07: ESP32 Hardware Control with Debounce Logic and Signal Filtering

## Overview
This project marks my transition from Arduino Uno to the **ESP32** microcontroller. The goal was to build a robust, non-blocking software architecture to handle digital inputs (button with hardware debouncing) and analog inputs (potentiometer with digital signal filtering).

Instead of using basic delay-based tutorials, this project implements a stable state machine using boolean flags and non-blocking timing via `millis()`. Additionally, it solves the common ESP32 analog noise issue using a software-defined mathematical filter.

---

## Key Engineering Concepts Learned

### 1. Advanced Input Handling & Debouncing
Mechanical buttons suffer from "contact bounce" (generating hundreds of micro-signals per millisecond). 
* **Solution:** Implemented a non-blocking interval check ($50\text{ ms}$) using `millis()`.
* **State Verification:** Used a double-flag system (`ButtonStatus` and `Button_flag`) to detect a single *edge transition* (press event), preventing multiple triggers if the button is held down.
* **Internal Pull-Up:** Configured `INPUT_PULLUP` to utilize the ESP32's internal resistors, simplifying the hardware circuit.

### 2. Analog Noise Reduction via Digital Filtering (EMA)
The ESP32's 12-bit ADC ($0 - 4095$) is highly sensitive, causing raw potentiometer readings to fluctuate due to power supply ripple and electromagnetic interference.
* **Solution:** Implemented a First-Order Low-Pass Filter, specifically an **Exponential Moving Average (EMA)** algorithm:
  $$Y_n = (X_n \cdot k) + (Y_{n-1} \cdot (1 - k))$$
  *Where $X_n$ is raw data, $Y_n$ is filtered output, and $k = 0.1$ is the smoothing factor.*
* This mathematical filter smooths out the noise perfectly without introducing noticeable latency.

### 3. ESP32 Architecture & Hardware Pitfalls
* **Data vs. Power Cables:** Discovered that standard micro-USB/Type-C cables can be power-only. Switched to a full data-sync cable for serial communication with macOS.
* **Strapping Pins:** Researched the ESP32 pinout and learned about "strapping pins" (GPIOs that dictate the boot mode during power-up), which requires careful consideration when designing external circuits.
* **Baud Rate Optimization:** Upgraded the Serial transmission speed to `115200 baud` to handle dense analog data streams effectively.

---

## Hardware Configuration

| Component | ESP32 GPIO | Mode | Description |
|--- |--- |--- |--- |
| **External LED** | GPIO 14 | `OUTPUT` | Status indicator (toggled via button) |
| **Tactile Button** | GPIO 25 | `INPUT_PULLUP` | Main controller input (active LOW) |
| **Potentiometer** | GPIO 34 | `ANALOG INPUT` | Analog data source for filtering test |

---

## Code Architecture

The firmware is written in clean, production-ready C++ style, separating the timing logic for different hardware peripherals.

The full sketch is available in the repository: [07_esp32_button_led.ino](07_esp32_button_led.ino)

```cpp
#define LED_PIN 14 
#define BUT_PIN 25 
#define POT_PIN 34 

// State Flags
boolean Button_flag = 0; 
boolean ButtonStatus = 0; 
boolean LED_flag = 0;

// Non-blocking Timing Intervals
unsigned long ButtonPreviousTime = 0;
const int ButInterval = 50;

unsigned long PotPreviousTime = 0;
const int PotInterval = 50;

// DSP Filter Variables
float filteredPotValue = 0; 
const float k = 0.1; // Smoothing factor (lower = smoother)

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUT_PIN, INPUT_PULLUP);

  // Initialize filter with the baseline reading
  filteredPotValue = analogRead(POT_PIN);
}

void loop() {
  unsigned long currentTime = millis();

  // --- Task 1: Debounced Button Scanning ---
  if (currentTime - ButtonPreviousTime >= ButInterval) {
    ButtonPreviousTime = currentTime;
    ButtonStatus = !digitalRead(BUT_PIN); // Inverted due to PULLUP
    
    if(ButtonStatus == 1 && Button_flag == 0) {
      Button_flag = 1; 
      LED_flag = !LED_flag; 
      digitalWrite(LED_PIN, LED_flag); 
    }
    if (ButtonStatus == 0 && Button_flag == 1) {
      Button_flag = 0;
    }
  }

  // --- Task 2: Filtered Analog Read & Logging ---
  if (currentTime - PotPreviousTime >= PotInterval) {
    PotPreviousTime = currentTime;
    int Pot_Data = analogRead(POT_PIN);
    
    // EMA Filtering Formula
    filteredPotValue = (Pot_Data * k) + (filteredPotValue * (1.0 - k)); 
    
    Serial.print("Raw data: ");
    Serial.print(Pot_Data);
    Serial.print(" | Data with filter: ");
    Serial.println(filteredPotValue);
  }
}
```
## Media & Hardware Demonstration

To maintain a high standard of documentation, the circuit design is presented in two ways: a clean schematic from the simulator for clear wiring reference, and a photo of the actual physical assembly to verify hardware functionality.

### 1. Circuit Schematic (Simulator)
For precise wiring details, component connections, and pinouts, refer to this digital schematic:

![Circuit Schematic](images/07_cirkitdesigner_scheme.png)

### 2. Physical Hardware Assembly
Here is the real-world setup built on the breadboard with the ESP32 and the custom breakout board. While physical photos can look cluttered due to wiring, this proves the code runs successfully on actual hardware:

![Physical Hardware Setup](images/07_real_photo.png)

### 3. Video Demonstration
You can watch the full demonstration of the project in action on YouTube. The video shows the non-blocking button state toggle (LED control) and the real-time Serial Monitor output demonstrating the Exponential Moving Average (EMA) filter smoothing out the potentiometer noise:

[![Watch the Demo on YouTube](https://img.shields.io/badge/YouTube-Video_Demo-red?style=for-the-badge&logo=youtube)](https://youtube.com/shorts/F9xz-rTDfB0)

## How to Run

- Connect the ESP32 to your computer using a data-capable USB cable.

- Open the project in the Arduino IDE or VS Code (PlatformIO).

- Select your ESP32 Development Module board and the correct serial port.

- Upload the sketch.

- Open the Serial Monitor, set the baud rate to 115200, and observe the clean, filtered analog signal alongside the stable button-triggered LED state.