//Motor
#define MOTOR1_AIN1 26 
#define MOTOR1_AIN2 25
#define STBY 27
#define PWMA 33
#define MOTOR2_BIN1 12 
#define MOTOR2_BIN2 14
#define PWMB 13 


//Encoder

#define MOTOR1_ENCODER_A 34
#define MOTOR1_ENCODER_B 39
#define MOTOR2_ENCODER_A 32 
#define MOTOR2_ENCODER_B 35 


volatile unsigned long M_Count1 = 0;
volatile unsigned long M_Count2 = 0;
unsigned long TempoUltimaInterrupcao = 0;
int MTA = 0;
int MTB = 0;
const float M_Encoder_PulsosPorRev = 900.00;

  // função para realizar a interrupção Motor 1
void ICACHE_RAM_ATTR MotorEncoderEvent1() {
  if (digitalRead(MOTOR1_ENCODER_A) == HIGH) {
    if (digitalRead(MOTOR1_ENCODER_B) == LOW) {
      M_Count1++;
    } else {
      M_Count1--;
    }
  } else {
    if (digitalRead(MOTOR1_ENCODER_B) == LOW) {
      M_Count1--;
    } else {
      M_Count1++;
    }
  }
}
  // função para realizar a interrupção
void ICACHE_RAM_ATTR MotorEncoderEvent2() {
  if (digitalRead(MOTOR2_ENCODER_B) == HIGH) {
    if (digitalRead(MOTOR2_ENCODER_A) == LOW) {
      M_Count2++;
    } else {
      M_Count2--;
    }
  } else {
    if (digitalRead(MOTOR2_ENCODER_A) == LOW) {
      M_Count2--;
    } else {
      M_Count2++;
    }
  }
}


void setup() {
  
  attachInterrupt(digitalPinToInterrupt(MOTOR1_ENCODER_A), MotorEncoderEvent1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOTOR2_ENCODER_B), MotorEncoderEvent2, CHANGE);
  
  
  pinMode(MOTOR1_ENCODER_A, INPUT_PULLUP);
  pinMode(MOTOR1_ENCODER_B, INPUT_PULLUP);
  pinMode(MOTOR2_ENCODER_A, INPUT_PULLUP);
  pinMode(MOTOR2_ENCODER_B, INPUT_PULLUP);
  
  pinMode(PWMA,OUTPUT);
  pinMode(MOTOR1_AIN1,OUTPUT);
  pinMode(MOTOR1_AIN2,OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(MOTOR2_BIN1,OUTPUT);
  pinMode(MOTOR2_BIN2,OUTPUT);
  pinMode(STBY,OUTPUT);  

  digitalWrite(STBY,HIGH); 
          
  Serial.begin(9600); 
  
  //Acionando motores
  digitalWrite(MOTOR1_AIN1, LOW);
  digitalWrite(MOTOR1_AIN2, HIGH);
  digitalWrite(MOTOR2_BIN1, LOW);
  digitalWrite(MOTOR2_BIN2, HIGH);  
  MTA = 80;
  MTB = 80; 
  analogWrite(PWMA,MTA);
  analogWrite(PWMB,MTB);


}

void loop() {
  
  unsigned long TempoAtual = millis();
  unsigned int M_Pulsos1;
  unsigned int M_Pulsos2;
  float Velocidade_Direita;
  float Velocidade_Esquerda;
  
  if (TempoAtual - TempoUltimaInterrupcao >= 100) {
    
    detachInterrupt(digitalPinToInterrupt(MOTOR1_ENCODER_A));
      M_Pulsos1 = M_Count1;    
      M_Count1 = 0;
    attachInterrupt(digitalPinToInterrupt(MOTOR1_ENCODER_A), MotorEncoderEvent1, CHANGE);
    
    Velocidade_Esquerda = (((float) M_Pulsos1 * 10) / M_Encoder_PulsosPorRev) * 60.0;
    
    detachInterrupt(digitalPinToInterrupt(MOTOR2_ENCODER_B));
      M_Pulsos2 = M_Count2;    
      M_Count2 = 0;
    attachInterrupt(digitalPinToInterrupt(MOTOR2_ENCODER_B), MotorEncoderEvent2, CHANGE);
    
    Velocidade_Direita = (((float) M_Pulsos2 * 10) / M_Encoder_PulsosPorRev) * 60.0;   

    TempoUltimaInterrupcao = TempoAtual;
  }

Serial.print("Velocidade Direita: ");
Serial.println(Velocidade_Direita);
Serial.print("Velocidade Esquerda: ");
Serial.println(Velocidade_Esquerda);
delay(500);

}


// if (Serial.available() > 0) {
//   int dado = Serial.read();
//   if (dado == 'a') {
//     digitalWrite(MOTOR1_AIN1, HIGH);
//     digitalWrite(MOTOR1_AIN2, LOW);
//     MTA = 80;
//     analogWrite(PWMA,MTA);

//   }
//   if (dado == 'b') {
//     digitalWrite(MOTOR1_AIN1, LOW);
//     digitalWrite(MOTOR1_AIN2, HIGH);
//     MTA = 80;
//     analogWrite(PWMA,MTA);                             

//   }
//   if (dado == 'c') {
//     digitalWrite(MOTOR1_AIN1, LOW);
//     digitalWrite(MOTOR1_AIN2, LOW);
//     MTA = 0;
//     analogWrite(PWMA,MTA);

//   }
// }

 




  
