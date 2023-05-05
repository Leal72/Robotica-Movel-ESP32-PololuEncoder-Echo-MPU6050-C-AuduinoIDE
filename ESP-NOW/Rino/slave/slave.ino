//sentido dos motores R3
#include <esp_now.h>
#include <WiFi.h>

#define ESP32_DEVKITV1 {0x24, 0x62, 0xAB, 0xD7, 0x4B, 0xA8}
#define ESP32S_1 {0xc0, 0x49, 0xef, 0xd3, 0x83, 0x84}
#define ESP32S_2 {0xc0, 0x49, 0xef, 0xd3, 0x6c, 0xe4}
#define ALL {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0FF}

uint8_t masterMacAddress[] = ESP32S_2;
#define ENCRYPT true 

float vRefE = 0, vRefD = 0;

#define vMax 1.67 // em m/s

#define PWMA 32
#define AIN2 33
#define AIN1 25
#define STBY 26
#define BIN1 27
#define BIN2 14
#define PWMB 12

int MTA = 0, MTB = 0;

static const char* PMK_KEY_STR = "RINOBOT";
static const char* LMK_KEY_STR = "RINOBOT";

typedef struct message {
    short int vel_left;
    short int vel_right;
} message;

message data{0, 0};

bool available = false;

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  Serial.print("Packet received.");
  memcpy(&data, incomingData, sizeof(data));
  available = true;
}

void setup() {
  Serial.begin(300);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  digitalWrite(STBY, HIGH);

  ledcAttachPin(PWMB, 0);
  ledcSetup(0, 20000, 12);
  ledcAttachPin(PWMA, 1);
  ledcSetup(1, 20000, 12);
  
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("There was an error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_pmk((uint8_t *)PMK_KEY_STR);
  
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));  
  peerInfo.channel = 0;
  memcpy(peerInfo.lmk, LMK_KEY_STR, 16);
  peerInfo.encrypt = ENCRYPT;
  memcpy(peerInfo.peer_addr, masterMacAddress, 6);

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if (available){
    available = false;
    vRefE = data.vel_left*0.01;
    vRefD = data.vel_right*0.01;
    
    Serial.print("Vel Esquerda: ");
    Serial.println(vRefE);
    Serial.print("Vel Direita: ");
    Serial.println(vRefD);

    // Ajuste do sentido de giro do motor esquerdo
    if (vRefE > 0)
    {
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
    }
    else if (vRefE < 0)
    {
      vRefE = -1 * (vRefE);
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
    }
    else
    {
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, HIGH);
    }

    // Ajuste do sentido de giro do motor direito
    if (vRefD > 0)
    {
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
    }
    else if (vRefD < 0)
    {
      vRefD = -1 * (vRefD);
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
    }
    else
    {
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, HIGH);
    }
    
    // Cálculo do PWM referente à velocidade desejada
    MTA = (int)((vRefD * 4096) / vMax);
    MTB = (int)((vRefE * 4096) / vMax);
      
    if (MTA > 4096) MTA = 4096;
    if (MTB > 4096) MTB = 4096;

    ledcWrite(0, MTA);
    ledcWrite(1, MTB);
    }
  }
