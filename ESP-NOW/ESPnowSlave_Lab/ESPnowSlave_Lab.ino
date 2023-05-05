#include "Arduino.h"
#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  char a_char[32];
  int b_int;
  float c_float;
  bool d_bool;
};

// Create a struct_message called myData
struct_message myData;

void setup() {
  Serial.begin(300);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("RX_1", "RX_1_Senha", CHANNEL , 0);

  esp_now_init();
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}
void loop() {
 
}


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a_char);
  Serial.print("Int: ");
  Serial.println(myData.b_int);
  Serial.print("Float: ");
  Serial.println(myData.c_float);
  Serial.print("Bool: ");
  Serial.println(myData.d_bool ? "true" : "false");
  Serial.println();
}


