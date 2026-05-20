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

// время для объезда
const int STOP_DURATION = 300; // останавливаем моторы если есть препятствие 
const int BACK_DURATION = 500; // время отъезда назад 
const int TURN_LEFT_DURATION = 400; // время поворота

// Для неблокирующего измерения
unsigned long lastMeasureTime = 0;
const unsigned long MEASURE_INTERVAL = 100; // измерять расстояние раз в 100 мс

// значение расстояния до объекта (начальное значение)
float dist = 500; // берём начальное расстояние до объекта 500 см с запасом, поскольку 400 см это предел для датчика HC-SR04

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

// ПОВОРОТ НАЛЕВО (на месте) 
void turnLeft() {
  // Левые колёса назад, правые вперёд
  digitalWrite(LeftFrontIN1, HIGH);
  digitalWrite(LeftFrontIN2, LOW);
  digitalWrite(LeftRearIN3,  HIGH);
  digitalWrite(LeftRearIN4,  LOW);
  
  digitalWrite(RightFrontIN1, LOW);
  digitalWrite(RightFrontIN2, HIGH);
  digitalWrite(RightRearIN3,  LOW);
  digitalWrite(RightRearIN4,  HIGH);
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
  return distance; // если получено конкреное расстояние (то есть датчик выдал какое-то значение), то возвращаем значение расстояния до объекта
}

void loop() {
  forward();  // drive forward
  unsigned long currentTime = micros();
  if (currentTime - lastMeasureTime >= MEASURE_INTERVAL) {
    lastMeasureTime = currentTime;
    float NEW_dist = getDistance();
    if (NEW_dist != 999) {
      dist = NEW_dist;
    }
  }
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
      // отъезжаем на расстояние примерно 30 см (безопасное расстояние)
      stopMotors();
      backward();
      delay(1500);

      // Теперь выполняем стандартный объезд (как при 10–30 см)
      delay(STOP_DURATION);
      backward();
      delay(BACK_DURATION);
      turnLeft();
      delay(TURN_LEFT_DURATION);
    }
    else if (dist <= 30.0) {
      // Obstacle at 10–30 cm
      stopMotors();
      delay(STOP_DURATION);
      backward();
      delay(BACK_DURATION);
      turnLeft();
      delay(TURN_LEFT_DURATION);
    }
  }
  
  // Short delay between loops for stability
  delay(50);
}