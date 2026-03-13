// ==========================================
// MAZE SOLVER + OBSTACLE AVOIDER + LINE FOLLOWER
// Priority: Obstacle > Left > Straight > Right
// ==========================================

// ---------- MOTOR PINS ----------
const int enA = 6;
const int in1 = 2;
const int in2 = 3;
const int in3 = 4;
const int in4 = 5;
const int enB = 11;

// ---------- SENSOR PINS ----------
const int irLeft   = 8;
const int irCenter = 9;
const int irRight  = 10;
const int trigPin  = A1;
const int echoPin  = A2;

// ---------- TUNABLE VALUES ----------
int speedForward = 100;
int speedTurn    = 130;
int stopDist     = 15;
int turnDelay    = 250;

void setup() {
  pinMode(enA, OUTPUT); pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT); pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);

  pinMode(irLeft, INPUT);
  pinMode(irCenter, INPUT);
  pinMode(irRight, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  stopMotors();
}

// ==========================================
// MAIN LOOP
// ==========================================
void loop() {

  // ---------- OBSTACLE CHECK ----------
  int distance = getDistance();
  if (distance > 0 && distance <= stopDist) {
    stopMotors();
    delay(300);
    uTurn();
    return;
  }

  // ---------- IR SENSOR READ ----------
  // BLACK = 0, WHITE = 1
  int L = digitalRead(irLeft);
  int C = digitalRead(irCenter);
  int R = digitalRead(irRight);

  // ---------- MAZE LOGIC (LEFT HAND RULE) ----------

  // Straight path
  if (L == 1 && C == 0 && R == 1) {
    moveForward();
  }

  // Left turn available (and center NOT available)
  else if (L == 0 && C == 1) {
    stopMotors();
    delay(50);
    turnLeft();
  }

  // Right turn only if left & center are blocked
  else if (L == 1 && C == 1 && R == 0) {
    stopMotors();
    delay(50);
    turnRight();
  }

  // End of maze / junction
  else if (L == 0 && C == 0 && R == 0) {
    stopMotors();
    delay(800);   // End reached
    while (1);    // STOP FOREVER
  }

  // Line lost / dead end
  else {
    uTurn();
  }
}

// ==========================================
// MOTOR FUNCTIONS
// ==========================================
void moveForward() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);  analogWrite(enA, speedForward);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);  analogWrite(enB, speedForward);
}

void turnLeft() {
  digitalWrite(in1, LOW);  digitalWrite(in2, HIGH); analogWrite(enA, speedTurn);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);  analogWrite(enB, speedTurn);
  delay(turnDelay);
}

void turnRight() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);  analogWrite(enA, speedTurn);
  digitalWrite(in3, LOW);  digitalWrite(in4, HIGH); analogWrite(enB, speedTurn);
  delay(turnDelay);
}

void uTurn() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);  analogWrite(enA, speedTurn);
  digitalWrite(in3, LOW);  digitalWrite(in4, HIGH); analogWrite(enB, speedTurn);
  delay(turnDelay * 2);
  stopMotors();
  delay(200);
}

void stopMotors() {
  digitalWrite(in1, LOW); digitalWrite(in2, LOW); analogWrite(enA, 0);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW); analogWrite(enB, 0);
}

// ==========================================
// ULTRASONIC FUNCTION
// ==========================================
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000); // timeout
  if (duration == 0) return 0;

  return duration * 0.034 / 2;
}
