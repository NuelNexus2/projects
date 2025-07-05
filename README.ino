# projects
const int S0 = 2;
const int S1 = 3;
const int S2 = 4;
const int S3 = 5;

const int ENA = 9;
const int IN1 = 8;
const int IN2 = 7;

const int ENB = 10;
const int IN3 = 12;
const int IN4 = 11;

float Kp = 25.0;
float Ki = 0.0;
float Kd = 15.0;

float error = 0;
float previous_error = 0;
float integral = 0;

int baseSpeed = 150;

void setup() {
  pinMode(S0, INPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int sensor[4] = {
    digitalRead(S0),
    digitalRead(S1),
    digitalRead(S2),
    digitalRead(S3)
  };

  float weights[4] = {-1.5, -0.5, 0.5, 1.5};
  float weighted_sum = 0;
  int active_sensors = 0;

  for (int i = 0; i < 4; i++) {
    weighted_sum += sensor[i] * weights[i];
    active_sensors += sensor[i];
  }

  float position = 0;
  if (active_sensors > 0) {
    position = weighted_sum / active_sensors;
  } else {
    position = previous_error > 0 ? 2 : -2;
  }

  error = 0 - position;
  integral += error;
  float derivative = error - previous_error;
  float correction = (Kp * error) + (Ki * integral) + (Kd * derivative);
  previous_error = error;

  int leftSpeed = baseSpeed - correction;
  int rightSpeed = baseSpeed + correction;

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);

  Serial.print("Sensors: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(sensor[i]);
    Serial.print(" ");
  }
  Serial.print(" | Pos: ");
  Serial.print(position);
  Serial.print(" | Error: ");
  Serial.print(error);
  Serial.print(" | L: ");
  Serial.print(leftSpeed);
  Serial.print(" | R: ");
  Serial.println(rightSpeed);

  delay(10);
}

