#include <esp_now.h>
#include <WiFi.h>

#define ESP32_DEVKITV1 {0x24, 0x62, 0xAB, 0xD7, 0x4B, 0xA8}
#define ESP32S_1 {0xC0, 0x49, 0xEF, 0xD3, 0x83, 0x84}
#define ESP32S_2 {0xC0, 0x49, 0xEF, 0xD3, 0x6C, 0xE4}
#define ALL {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0FF}

const int ROBOT[] = {1}; // MODIFICAR
const uint8_t ADDRESS[][6] = {ESP32S_1}; // MODIFICAR
#define NUM_ROBOTS 1
#define ENCRYPT true 

static const char* PMK_KEY_STR = "RINOBOT";
static const char* LMK_KEY_STR = "RINOBOT";

typedef struct message {
    short int vel_left;
    short int vel_right;
} message;

message data;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Esp-Now");
  status == ESP_NOW_SEND_SUCCESS ? digitalWrite(2, HIGH) : digitalWrite(2, LOW);
}
 
void setup() {
  Serial.begin(300);

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
  for(int i=0;i<NUM_ROBOTS;i++)
  {
    memcpy(peerInfo.peer_addr, ADDRESS[i], 6);
          
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }
  }
  
  esp_now_register_send_cb(OnDataSent);
  
  pinMode(2, OUTPUT);
}

void loop() {
  if(Serial.available() > 6){
    
    int robot=Serial.read();
    int vl_s=Serial.read();
    int vl_d=Serial.read();
    int vl_u=Serial.read();
    int vr_s=Serial.read();
    int vr_d=Serial.read();
    int vr_u=Serial.read();
    
    data.vel_left = vl_s ? -(10*vl_d + vl_u) : 10*vl_d + vl_u;
    data.vel_right = vr_s ? -(10*vr_d + vr_u) : 10*vr_d + vr_u;
    for (int i=0; i<NUM_ROBOTS; i++){
      //if (true){
      if (robot==ROBOT[i]){
        esp_now_send(ADDRESS[i], (uint8_t *)&data, sizeof(data));
        break;
      }
    }
  }
}    
