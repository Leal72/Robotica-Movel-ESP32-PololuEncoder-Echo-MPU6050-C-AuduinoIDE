#include <SimpleKalmanFilter.h>

SimpleKalmanFilter leftEncKalman (1, 2, 0.11);
SimpleKalmanFilter rightEncKalman(1, 2, 0.11);

//Encoder
#define MOTOR1_ENCODER_A 34
#define MOTOR1_ENCODER_B 39
#define MOTOR2_ENCODER_A 35 
#define MOTOR2_ENCODER_B 32 

volatile long contEncEsq = 0;                
volatile long contEncDir = 0;
volatile long distEncEsq = 0;
volatile long distEncDir = 0;
// volatile long lastContEncEsq = 0;
// volatile long lastContEncDir = 0;
// volatile long lastTimeEsq = 0;
// volatile long lastTimeDir = 0;
// const float diametroRoda = 5;


void encEsqA() {
  if (digitalRead(MOTOR1_ENCODER_B) != digitalRead(MOTOR1_ENCODER_A)) {
    contEncEsq ++;
    distEncEsq ++;
  } else {
    contEncEsq --;
    distEncEsq --;
  }
}

void encEsqB() {
  if (digitalRead(MOTOR1_ENCODER_B) == digitalRead(MOTOR1_ENCODER_A)) {
    contEncEsq ++;
    distEncEsq ++;
  } else {
    contEncEsq --;
    distEncEsq --;
  }
}

void encDirA() {
  if (digitalRead(MOTOR2_ENCODER_B) != digitalRead(MOTOR2_ENCODER_A)) {
    contEncDir ++;
    distEncDir ++;
  } else {
    contEncDir --;
    distEncDir --;
  }
}

void encDirB() {
  if (digitalRead(MOTOR2_ENCODER_B) == digitalRead(MOTOR2_ENCODER_A)) {
    contEncDir ++;
    distEncDir ++;
  } else {
    contEncDir --;
    distEncDir --;
  }
}

void updateLeftEncoder() {
  long dX = contEncEsq - lastContEncEsq;
  long dT = micros() - lastTimeEsq;
  if (dX == 0) {
    if (dT >= 20000 && dX == 0) {
      if (kalmanEncoders) velEncEsq = leftEncKalman.updateEstimate(0);
      else velEncEsq = 0;
    } else if (kalmanEncoders) velEncEsq = leftEncKalman.updateEstimate(realLeftSpeed);
    return;
  }
  velEncEsq = dX * 1000000.0 / (120.0 * dT);
  if (si) velEncEsq = (float) PI * diametroRoda * velEncEsq;
  realLeftSpeed = velEncEsq;
  if (kalmanEncoders) velEncEsq = leftEncKalman.updateEstimate(velEncEsq);
  lastContEncEsq = contEncEsq;
  lastTimeEsq = micros();
}

void updateRightEncoder() {
  long dX = contEncDir - lastContEncDir;
  long dT = micros() - lastTimeDir;
  if (dX == 0) {
    if (dT >= 20000 && dX == 0) {
      if (kalmanEncoders) velEncDir = rightEncKalman.updateEstimate(0);
      else velEncDir = 0;
    } else if (kalmanEncoders) velEncDir = rightEncKalman.updateEstimate(realRightSpeed);
    return;
  }
  velEncDir = dX * 1000000.0 / (120.0 * dT);
  if (si) velEncDir = (float) PI * diametroRoda * velEncDir;
  realRightSpeed = velEncDir;
  if (kalmanEncoders) velEncDir = rightEncKalman.updateEstimate(velEncDir);
  lastContEncDir = contEncDir;
  lastTimeDir = micros();
}

void updateEncoders() {
  updateLeftEncoder();
  updateRightEncoder();
}

void setup() {
  
  pinMode(MOTOR1_ENCODER_A, INPUT_PULLUP);
  pinMode(MOTOR1_ENCODER_B, INPUT_PULLUP);
  pinMode(MOTOR2_ENCODER_A, INPUT_PULLUP);
  pinMode(MOTOR2_ENCODER_B, INPUT_PULLUP);
 
 
  attachInterrupt(digitalPinToInterrupt(MOTOR1_ENCODER_A), encEsqA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOTOR1_ENCODER_B), encEsqB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOTOR2_ENCODER_A), encDirA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOTOR2_ENCODER_B), encDirB, CHANGE);

  Serial.begin(115200);

}

void loop() {
  Serial.print("Contador Direita: ");
  Serial.println(contEncDir);
  Serial.print("Contador Esquerda: ");
  Serial.println(contEncEsq);

}
