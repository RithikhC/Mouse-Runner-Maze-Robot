/* * Black Line Follower - Single Sensor
 * Logic: Follows line when detected. When lost, it performs 
 * an alternating "zig-zag" search with an increasing radius.
 */

// Motor Pin Definitions
const int IN1 = 2;  // Right Forward
const int IN2 = 3;  // Right Backward
const int IN3 = 4;  // Left Forward
const int IN4 = 5;  // Left Backward
const int ENA = 6;  // Speed Control Right (PWM)
const int ENB = 11; // Speed Control Left (PWM)

// Sensor Pin
const int IR_SENSOR = 9; 

// Speed Settings (0-255)
const int driveSpeed = 160;   
const int searchSpeed = 140;  

// Search Logic Variables
unsigned long currentSearchTime = 300; // Initial time to look in one direction
const int timeIncrement = 250;         // Added to time every direction swap
bool searchRight = true;               // Track which way to turn next

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IR_SENSOR, INPUT);
  
  // Optional: Serial.begin(9600); // For debugging sensor values
}

void loop() {
  // Read sensor: LOW = Black Line, HIGH = White Floor
  // If your sensor is inverted, change LOW to HIGH
  if (digitalRead(IR_SENSOR) == LOW) {
    moveForward();
    
    // Reset search variables because we found the line
    currentSearchTime = 300; 
    searchRight = true; 
  } else {
    performAlternatingSearch();
  }
}

// --- Movement Functions ---

void moveForward() {
  analogWrite(ENA, driveSpeed);
  analogWrite(ENB, driveSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void performAlternatingSearch() {
  unsigned long startTime = millis();

  // Stay in this loop while the sensor sees white (HIGH)
  while (digitalRead(IR_SENSOR) == HIGH) {
    analogWrite(ENA, searchSpeed);
    analogWrite(ENB, searchSpeed);

    if (searchRight) {
      // Spin Right (Right wheel back, Left wheel forward)
      digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    } else {
      // Spin Left (Right wheel forward, Left wheel back)
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    }

    // Check if we have searched in this direction long enough
    if (millis() - startTime > currentSearchTime) {
      stopRobot();
      delay(100); // Short pause to prevent motor strain
      
      // Expand the search for the next attempt
      currentSearchTime += timeIncrement; 
      searchRight = !searchRight; // Flip direction
      return; // Exit to main loop to check sensor again
    }
  }
}

void stopRobot() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}