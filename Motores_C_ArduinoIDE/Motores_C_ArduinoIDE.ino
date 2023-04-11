

#define MOTOR1_AIN1 26 
#define MOTOR1_AIN2 25
#define STBY 27
#define PWMA 33
#define MOTOR2_BIN1 14
#define MOTOR2_BIN2 12
#define PWMB 13


int MTA = 0;
int MTB = 0;

void setup() {
 
  pinMode(PWMA,OUTPUT);
  pinMode(MOTOR1_AIN1,OUTPUT);
  pinMode(MOTOR1_AIN2,OUTPUT);

  pinMode(PWMB,OUTPUT);
  pinMode(MOTOR2_BIN1,OUTPUT);
  pinMode(MOTOR2_BIN2,OUTPUT);
  pinMode(STBY,OUTPUT);
  
  digitalWrite(STBY,HIGH); 
          
  Serial.begin(9600); 
  

}

void loop() {


if (Serial.available() > 0) {
  int dado = Serial.read();
  if (dado == 'a') {
  digitalWrite(MOTOR1_AIN1, LOW);
  digitalWrite(MOTOR1_AIN2, HIGH);
  digitalWrite(MOTOR2_BIN1, LOW);
  digitalWrite(MOTOR2_BIN2, HIGH);

  // definir as velocidades dos motores
  MTA = 80;
  MTB = 80;
  
  analogWrite(PWMA,MTA);
  analogWrite(PWMB,MTB);;

  }
  if (dado == 'b') {
  // girar os motores para trás
  digitalWrite(MOTOR1_AIN1, HIGH);
  digitalWrite(MOTOR1_AIN2, LOW);
  digitalWrite(MOTOR2_BIN1, HIGH);
  digitalWrite(MOTOR2_BIN2, LOW);

  // definir as velocidades dos motores
  MTA = 80;
  MTB = 80;
  
  analogWrite(PWMA,MTA);
  analogWrite(PWMB,MTB);


  }
  if (dado == 'c') {
    digitalWrite(MOTOR1_AIN1, LOW);
    digitalWrite(MOTOR1_AIN2, LOW);
    digitalWrite(MOTOR2_BIN1, LOW);
    digitalWrite(MOTOR2_BIN2, LOW);
    MTA = 0;
    MTB = 0;
    analogWrite(PWMA,MTA);
    analogWrite(PWMB,MTB);

  }
}
}






  
