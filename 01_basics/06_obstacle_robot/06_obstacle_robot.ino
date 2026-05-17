// DRIVER #1 (LEFT side)
// Motor A left (front)
const byte LeftFrontIN1 = 2;
const byte LeftFrontIN2 = 3;

// Motor B left (rear)
const byte LeftRearIN3  = 4;
const byte LeftRearIN4  = 5;

// DRIVER #2 (RIGHT side)
// Motor A right (front)
const byte RightFrontIN1 = 6;
const byte RightFrontIN2 = 7;

// Motor B right (rear)
const byte RightRearIN3  = 8;
const byte RightRearIN4  = 9;

// HC-SR04
const byte TRIGPIN = 10;
const byte ECHOPIN = 11;

void setup() {
  // Set all pins as outputs
  pinMode(LeftFrontIN1, OUTPUT);
  pinMode(LeftFrontIN2, OUTPUT);
  pinMode(LeftRearIN3,  OUTPUT);
  pinMode(LeftRearIN4,  OUTPUT);
  
  pinMode(RightFrontIN1, OUTPUT);
  pinMode(RightFrontIN2, OUTPUT);
  pinMode(RightRearIN3,  OUTPUT);
  pinMode(RightRearIN4,  OUTPUT);
  
  // HC-SR04
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  Serial.begin(9600); // baud rate
  
  // Small delay before start
  delay(1000);
}

// FORWARD
void forward() {
  // Left side
  digitalWrite(LeftFrontIN1, LOW);
  digitalWrite(LeftFrontIN2, HIGH);
  digitalWrite(LeftRearIN3,  LOW);
  digitalWrite(LeftRearIN4,  HIGH);
  
  // Right side
  digitalWrite(RightFrontIN1, LOW);
  digitalWrite(RightFrontIN2, HIGH);
  digitalWrite(RightRearIN3,  LOW);
  digitalWrite(RightRearIN4,  HIGH);
}

// BACKWARD
void backward() {
  // Left side
  digitalWrite(LeftFrontIN1, HIGH);
  digitalWrite(LeftFrontIN2, LOW);
  digitalWrite(LeftRearIN3,  HIGH);
  digitalWrite(LeftRearIN4,  LOW);
  
  // Right side
  digitalWrite(RightFrontIN1, HIGH);
  digitalWrite(RightFrontIN2, LOW);
  digitalWrite(RightRearIN3,  HIGH);
  digitalWrite(RightRearIN4,  LOW);
}

// STOP
void stopMotors() {
  digitalWrite(LeftFrontIN1, LOW);
  digitalWrite(LeftFrontIN2, LOW);
  digitalWrite(LeftRearIN3,  LOW);
  digitalWrite(LeftRearIN4,  LOW);
  
  digitalWrite(RightFrontIN1, LOW);
  digitalWrite(RightFrontIN2, LOW);
  digitalWrite(RightRearIN3,  LOW);
  digitalWrite(RightRearIN4,  LOW);
}

// MEASURE DISTANCE
float getDistance() {
  // Trigger cleanup
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  // fire the sensor
  digitalWrite(TRIGPIN, HIGH); // turn on HC-SR04
  delayMicroseconds(10);       // per datasheet, wait 10 µs so HC-SR04 recognizes the signal from Arduino, not noise
  digitalWrite(TRIGPIN, LOW);  // turn off
  
  unsigned long duration = pulseIn(ECHOPIN, HIGH, 30000); // time during which ECHOPIN was HIGH
  float distance = (duration * 0.0343) / 2; // calculate distance in cm
  
  if (duration == 0) return 999;  // no echo
  return distance;
}

void loop() {
  forward();  // drive forward
  
  float dist = getDistance();
  
  // Debug output
  if (dist < 400) {
    Serial.print("Distance: ");
    Serial.print(dist);
    Serial.println(" cm");
  }

  // Obstacle check
  if (dist >= 2 && dist <= 400) { 
    // measurement range of HC-SR04
    if (dist < 10.0) { 
      // Emergency – too close
      stopMotors();
      delay(500);
      backward();
      delay(1500);
    }
    else if (dist <= 30.0) {
      // Obstacle at 10–30 cm
      stopMotors();
      delay(2000);
      backward();
      delay(1000);
    }
  }
  
  // Short delay between loops for stability
  delay(50);
}