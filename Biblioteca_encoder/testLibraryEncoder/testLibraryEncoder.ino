//#include "Arduino.h"
#include "PololuMagneticEncoder.h"

#define MOTOR1_AIN1 26 
#define MOTOR1_AIN2 25
#define STBY 27
#define PWMA 33
#define MOTOR2_BIN1 14
#define MOTOR2_BIN2 12
#define PWMB 13


PololuMagneticEncoder encoders;
int dt = 200;
long tempo = 0;
int MTA = 0;
int MTB = 0;

void setup() 
{
  pinMode(PWMA,OUTPUT);
  pinMode(MOTOR1_AIN1,OUTPUT);
  pinMode(MOTOR1_AIN2,OUTPUT);

  pinMode(PWMB,OUTPUT);
  pinMode(MOTOR2_BIN1,OUTPUT);
  pinMode(MOTOR2_BIN2,OUTPUT);
  pinMode(STBY,OUTPUT);

  
  Serial.begin(115200);
  encoders.setupEncoders(35, 32, 34, 39);

  digitalWrite(STBY,HIGH);   
  digitalWrite(MOTOR1_AIN2, LOW);
  digitalWrite(MOTOR1_AIN1, HIGH);
  digitalWrite(MOTOR2_BIN2, LOW);
  digitalWrite(MOTOR2_BIN1, HIGH);

  // definir as velocidades dos motores
  MTA = 255;
  MTB = 255;
  
  analogWrite(PWMA,MTA);
  analogWrite(PWMB,MTB);

  // delay(189.4736842);

  // digitalWrite(STBY,LOW);

}

void loop() 
{
  if(millis()- tempo >= dt )
  {
    tempo = millis();
    
    Serial.print("Pulsos Esquerda: ");
    Serial.println(encoders.getCountsEncoderLeft());
    encoders.getCountsAndResetEncoderLeft();
    
    Serial.print("Pulsos Direita: ");
    Serial.println(encoders.getCountsEncoderRight());
    encoders.getCountsAndResetEncoderRight();
  }


}
