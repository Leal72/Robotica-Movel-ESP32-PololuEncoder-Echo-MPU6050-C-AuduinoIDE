#include <esp_now.h>
#include <WiFi.h>

#define ESP32_DEVKITV1 {0x24, 0x62, 0xab, 0xd7, 0x4b, 0xa8}
#define ESP32S_1 {0xc0, 0x49, 0xef, 0xd3, 0x83, 0x84}
#define ESP32S_2 {0xc0, 0x49, 0xef, 0xd3, 0x6c, 0xe4}
#define ALL {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}


// macAddress do ESP32 RX escolhido
uint8_t broadcastAddress[] = ESP32S_1;


// Estrutura para armazenas os valores a serem envidados 
typedef struct struct_message {
  short int vRef_left;
  short int vRef_right;
};

// Criando estrutura chamada myData
struct_message myData;

// Callback quando mensagem enviada
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  status == ESP_NOW_SEND_SUCCESS ? digitalWrite(2, HIGH) : digitalWrite(2, LOW);
}

void setup() {
  // Iniciando comunicação serial 
  Serial.begin(300);

  pinMode(2, OUTPUT);    //Led 

  Serial.println("");
  delay(500);
 
  Serial.println("*** START ***");

  // Colocando ESP em modo WiFi station
  WiFi.mode(WIFI_STA);

  // Iniciando ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  //Com ESP-NOW iniciado, registra a função de CallBack para receber status da mensagem  
  esp_now_register_send_cb(OnDataSent);

  // Registrando par qual será enviada a mensagem 
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false; // Não incluindo uma cryptografia na mensagem 

  // Adicionado par
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    while(1);
  }
  Serial.println("Completed");  

  
}

void loop() {
  // Setando valor as variavel enviadas
  myData.vRef_left = random(-5, 5);
  myData.vRef_right = random(-5, 5);


  // Sentando mensagem no monitor serial informando os valores enviados 
  Serial.print("Velocidade de referencia esquerda: ");
  Serial.println(myData.vRef_left);
  Serial.print("Velocidade de referencia direita:");
  Serial.println(myData.vRef_right);

  // Status da mensagem    
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(10000);
}