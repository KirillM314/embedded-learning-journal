// DRIVER #1 (LEFT side)
// Motor A left (front)
int leftFrontIN1 = 2;
int leftFrontIN2 = 3;

// Motor B left (rear)
int leftRearIN3 = 4;
int leftRearIN4 = 5;

// DRIVER #2 (RIGHT side)
// Motor A right (front)
int rightFrontIN1 = 6;
int rightFrontIN2 = 7;

// Motor B right (rear)
int rightRearIN3 = 8;
int rightRearIN4 = 9;

void setup() {
  // Set all pins as outputs
  pinMode(leftFrontIN1, OUTPUT);
  pinMode(leftFrontIN2, OUTPUT);
  pinMode(leftRearIN3, OUTPUT);
  pinMode(leftRearIN4, OUTPUT);
  
  pinMode(rightFrontIN1, OUTPUT);
  pinMode(rightFrontIN2, OUTPUT);
  pinMode(rightRearIN3, OUTPUT);
  pinMode(rightRearIN4, OUTPUT);
  
  // Small delay before start
  delay(1000);
}

// MOVEMENT FUNCTIONS 

// Forward (all 4 motors)
void forward() {
  digitalWrite(leftFrontIN1, HIGH);
  digitalWrite(leftFrontIN2, LOW);
  digitalWrite(leftRearIN3, HIGH);
  digitalWrite(leftRearIN4, LOW);
  
  digitalWrite(rightFrontIN1, HIGH);
  digitalWrite(rightFrontIN2, LOW);
  digitalWrite(rightRearIN3, HIGH);
  digitalWrite(rightRearIN4, LOW);
}

// Backward
void backward() {
  digitalWrite(leftFrontIN1, LOW);
  digitalWrite(leftFrontIN2, HIGH);
  digitalWrite(leftRearIN3, LOW);
  digitalWrite(leftRearIN4, HIGH);
  
  digitalWrite(rightFrontIN1, LOW);
  digitalWrite(rightFrontIN2, HIGH);
  digitalWrite(rightRearIN3, LOW);
  digitalWrite(rightRearIN4, HIGH);
}

// Stop
void stop() {
  digitalWrite(leftFrontIN1, LOW);
  digitalWrite(leftFrontIN2, LOW);
  digitalWrite(leftRearIN3, LOW);
  digitalWrite(leftRearIN4, LOW);
  
  digitalWrite(rightFrontIN1, LOW);
  digitalWrite(rightFrontIN2, LOW);
  digitalWrite(rightRearIN3, LOW);
  digitalWrite(rightRearIN4, LOW);
}

// MAIN LOOP

void loop() {
  forward();      // Drive forward
  delay(2000);    // 2 seconds
  
  stop();         // Stop
  delay(1000);    // 1 second
  
  backward();     // Drive backward
  delay(2000);    // 2 seconds
  
  stop();         // Stop
  delay(1000);    // 1 second
}