#include "Arduino.h"
#include <esp_now.h>
#include <WiFi.h>

#define ESP32_DEVKITV1 {0x24, 0x62, 0xab, 0xd7, 0x4b, 0xa8}
#define ESP32S_1 {0xc0, 0x49, 0xef, 0xd3, 0x83, 0x84}
#define ESP32S_2 {0xc0, 0x49, 0xef, 0xd3, 0x6c, 0xe4}
#define ALL {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}

#define MOTOR1_AIN1 26 
#define MOTOR1_AIN2 25
#define STBY 27
#define PWMA 33
#define MOTOR2_BIN1 14
#define MOTOR2_BIN2 12
#define PWMB 13

uint8_t mastermacAddress[] = ESP32S_2;

float vRef_E = 0, vRef_D = 0, vMax = 1.67;
int MTA = 0, MTB = 0;
bool available = false;


// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  short int vRef_left;
  short int vRef_right;
};

// Create a struct_message called myData
struct_message myData{0, 0};

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  Serial.print("Dado Recebido.");
  memcpy(&myData, incomingData, sizeof(myData));
  available = true;
}

void setup() {
  pinMode(PWMA,OUTPUT);
  pinMode(MOTOR1_AIN1,OUTPUT);
  pinMode(MOTOR1_AIN2,OUTPUT);

  pinMode(PWMB,OUTPUT);
  pinMode(MOTOR2_BIN1,OUTPUT);
  pinMode(MOTOR2_BIN2,OUTPUT);
  pinMode(STBY,OUTPUT);
  
  Serial.begin(300);

  digitalWrite(STBY,LOW);  
  digitalWrite(MOTOR1_AIN1, LOW);
  digitalWrite(MOTOR1_AIN2, LOW);
  digitalWrite(MOTOR2_BIN1, LOW);
  digitalWrite(MOTOR2_BIN2, LOW);
  analogWrite(PWMA,MTA);
  analogWrite(PWMB,MTB);  

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //   // Register peer
  // esp_now_peer_info_t peerInfo;
  // peerInfo.channel = 0;
  // peerInfo.encrypt = false;
  // memcpy(peerInfo.peer_addr, mastermacAddress, 6);

  // // Add peer
  // if (esp_now_add_peer(&peerInfo) != ESP_OK) {
  //   Serial.println("Failed to add peer");
  //   return;
  // }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if(available){
  available = false;

  vRef_E = myData.vRef_left*0.02; 
  vRef_D = myData.vRef_right*0.02;

  Serial.print("Velocidade de referencia esquerda: ");
  Serial.println(vRef_E);
  Serial.print("Velocidade de referencia direita: ");
  Serial.println(vRef_D);
  Serial.println();

  if(vRef_E > 0){
    
    digitalWrite(MOTOR1_AIN1, LOW);
    digitalWrite(MOTOR1_AIN2, HIGH);
    
  }  else if(vRef_E < 0){
    
    vRef_E = abs(vRef_E);
    digitalWrite(MOTOR1_AIN1, HIGH);
    digitalWrite(MOTOR1_AIN2, LOW);
      
  } else {
    
    digitalWrite(MOTOR1_AIN1, LOW);
    digitalWrite(MOTOR1_AIN2, LOW);
        
  }

  if(vRef_D > 0){
    
    digitalWrite(MOTOR2_BIN1, LOW);
    digitalWrite(MOTOR2_BIN2, HIGH);
    
  }  else if(vRef_D < 0){
    
    vRef_D = abs(vRef_D);
    digitalWrite(MOTOR2_BIN1, HIGH);
    digitalWrite(MOTOR2_BIN2, LOW);
      
  } else {
    
    digitalWrite(MOTOR2_BIN1, LOW);
    digitalWrite(MOTOR2_BIN2, LOW);
        
  }
  MTA = (int)((vRef_E * 255) / vMax);
  MTB = (int)((vRef_E * 255) / vMax);

  if(MTA >255){
    MTA = 255;
  }  
  if(MTB >255){
    MTB = 255;
  } 
  
  analogWrite(PWMA,MTA);
  analogWrite(PWMB,MTB);
  
  }

}
