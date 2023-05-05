
#include <esp_now.h>
#include <WiFi.h>


#define CHANNEL 1

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a_char[32];
  int b_int;
  float c_float;
  bool d_bool;
};

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t slave;


void setup() {
  Serial.begin(300);
  
  pinMode(2, OUTPUT);
  
  WiFi.mode(WIFI_STA);
  esp_now_init();
  // Init ESP-NOW  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
  ScanForSlave();
  esp_now_add_peer(&slave);
}

void loop() {

  // Set values to send
  strcpy(myData.a_char, "THIS IS A CHAR");
  myData.b_int = random(1, 20);
  myData.c_float = (float)random(10.0, 100.0) / 10.0;
  myData.d_bool = random(1, 10) < 5 ? true : false;

  // Send message via ESP-NOW
  Serial.println("Bytes transmitted: ");
  Serial.print("Char: ");
  Serial.println(myData.a_char);
  Serial.print("Int: ");
  Serial.println(myData.b_int);
  Serial.print("Float: ");
  Serial.println(myData.c_float);
  Serial.print("Bool: ");
  Serial.println(myData.d_bool ? "true" : "false");
  esp_err_t result = esp_now_send(slave.peer_addr, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(5000);
}

// Scan for slaves in AP mode
void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();

    for (int i = 0; i < scanResults; ++i) {

      String SSID = WiFi.SSID(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      if (SSID.indexOf("RX") == 0) {

        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }

        slave.channel = CHANNEL; 
        slave.encrypt = 0; 

        break;
      }
    }
  }




void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  status == ESP_NOW_SEND_SUCCESS ? digitalWrite(2, HIGH) : digitalWrite(2, LOW);
}



