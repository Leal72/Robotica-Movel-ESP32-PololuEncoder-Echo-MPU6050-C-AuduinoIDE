
const int TRIGGER_PIN = 23;  // Pin do Trigger do sensor
const int ECHO_PIN = 22;     // Pin do Echo do sensor

void setup() {
  Serial.begin(9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1; // Calcula a distância em centímetros
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);
}