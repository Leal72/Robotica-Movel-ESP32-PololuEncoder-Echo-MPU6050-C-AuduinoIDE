//Motor
#define MOTOR1_AIN1 26 
#define MOTOR1_AIN2 25
#define STBY 27
#define PWMA 33
#define MOTOR2_BIN1 14 
#define MOTOR2_BIN2 12
#define PWMB 13 


//Encoder
#define MOTOR1_ENCODER_A 34
#define MOTOR1_ENCODER_B 39
#define MOTOR2_ENCODER_A 35 
#define MOTOR2_ENCODER_B 32 


volatile signed long int M_Count1 = 0;
volatile signed long int M_Count2 = 0;
unsigned long TempoUltimaInterrupcao = 0;
int MTA = 0;
int MTB = 0;
const float M_Encoder_PulsosPorRev = 450.00;
// função para realizar a interrupção Motor 1


void ICACHE_RAM_ATTR MotorEncoderEventRighA() {
  if (digitalRead(MOTOR1_ENCODER_A) == HIGH) {
    if (digitalRead(MOTOR1_ENCODER_B) == LOW) {
      M_Count1--;
    } else {
      M_Count1++;
    }
  } else {
    if (digitalRead(MOTOR1_ENCODER_B) == LOW) {
      M_Count1++;
    } else {
      M_Count1--;
    }
  }
}
// void ICACHE_RAM_ATTR MotorEncoderEventRighB() {
//   if (digitalRead(MOTOR1_ENCODER_B) == HIGH) {
//     if (digitalRead(MOTOR1_ENCODER_A) == LOW) {
//       M_Count1--;
//     } else {
//       M_Count1++;
//     }
//   } else {
//     if (digitalRead(MOTOR1_ENCODER_A) == LOW) {
//       M_Count1++;
//     } else {
//       M_Count1--;
//     }
//   }

// // função para realizar a interrupção no motor 2
void ICACHE_RAM_ATTR MotorEncoderEventlLeftA() {
  if (digitalRead(MOTOR2_ENCODER_A) == HIGH) {
    if (digitalRead(MOTOR2_ENCODER_B) == LOW) {
      M_Count2--;
    } else {
      M_Count2++;
    }
  } else {
    if (digitalRead(MOTOR2_ENCODER_B) == LOW) {
      M_Count2++;
    } else {
      M_Count2--;
    }
  }
}
// void ICACHE_RAM_ATTR MotorEncoderEventlLeftB() {
//   if (digitalRead(MOTOR2_ENCODER_A) == HIGH) {
//     if (digitalRead(MOTOR2_ENCODER_B) == LOW) {
//       M_Count2--;
//     } else {
//       M_Count2++;
//     }
//   } else {
//     if (digitalRead(MOTOR2_ENCODER_B) == LOW) {
//       M_Count2++;
//     } else {
//       M_Count2--;
//     }
//   }
// }


void setup() {
  

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
  digitalWrite(MOTOR1_AIN1, LOW);
  digitalWrite(MOTOR1_AIN2, LOW);
  digitalWrite(MOTOR2_BIN1, LOW);
  digitalWrite(MOTOR2_BIN2, LOW);
  analogWrite(PWMA,MTA);
  analogWrite(PWMB,MTB); 

  attachInterrupt(digitalPinToInterrupt(MOTOR1_ENCODER_A), MotorEncoderEventRighA, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(MOTOR1_ENCODER_B), MotorEncoderEventRighB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOTOR2_ENCODER_A), MotorEncoderEventlLeftA, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(MOTOR2_ENCODER_B), MotorEncoderEventlLeftB, CHANGE);   
          
  Serial.begin(115200); 
  


}

void loop() {
  
  unsigned long TempoAtual = millis();
  signed long int M_Pulsos1;
  signed long int M_Pulsos2;
  float Velocidade_Direita;
  float Velocidade_Esquerda;

  // //Acionando motores RÉ
  // digitalWrite(MOTOR1_AIN1, HIGH);
  // digitalWrite(MOTOR1_AIN2, LOW);
  // digitalWrite(MOTOR2_BIN1, HIGH);
  // digitalWrite(MOTOR2_BIN2, LOW);  
  // MTA = 255;
  // MTB = 255; 
  // analogWrite(PWMA,MTA);
  // analogWrite(PWMB,MTB);


  //Acionando motores frente  
    digitalWrite(MOTOR1_AIN1, LOW);
    digitalWrite(MOTOR1_AIN2, HIGH);
    digitalWrite(MOTOR2_BIN1, LOW);
    digitalWrite(MOTOR2_BIN2, HIGH);  
    MTA = 255;
    MTB = 255; 
    analogWrite(PWMA,MTA);
    analogWrite(PWMB,MTB);

  
  if (TempoAtual - TempoUltimaInterrupcao >= 100) {
   
    
    detachInterrupt(digitalPinToInterrupt(MOTOR1_ENCODER_A));
      M_Pulsos1 =  M_Count1;    
      M_Count1 = 0;
    attachInterrupt(digitalPinToInterrupt(MOTOR1_ENCODER_A), MotorEncoderEventRighA, CHANGE);
    
    Velocidade_Esquerda = abs((((M_Pulsos1 * 10) / M_Encoder_PulsosPorRev) * 60.0));
    
    detachInterrupt(digitalPinToInterrupt(MOTOR2_ENCODER_A));
      M_Pulsos2 = M_Count2;    
      M_Count2 = 0;
    attachInterrupt(digitalPinToInterrupt(MOTOR2_ENCODER_A), MotorEncoderEventlLeftA, CHANGE);
    
    Velocidade_Direita = abs((((M_Pulsos2 * 10) / M_Encoder_PulsosPorRev) * 60.0));   

    TempoUltimaInterrupcao = TempoAtual;
  }

// Serial.print("Contador Direita: ");
// Serial.println(M_Count1);
// Serial.print("Contador Esquerda: ");
// Serial.println(M_Count2);
Serial.print("Pulsos Direita: ");
Serial.println(M_Pulsos1);
Serial.print("Pulsos Esquerda: ");
Serial.println(M_Pulsos2);
Serial.print("Velocidade Direita: ");
Serial.println(Velocidade_Direita);
Serial.print("Velocidade Esquerda: ");
Serial.println(Velocidade_Esquerda);
delay(500);

}
