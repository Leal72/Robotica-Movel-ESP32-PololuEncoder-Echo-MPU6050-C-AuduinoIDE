
#include <WiFi.h>

#define MOTOR1_AIN1 26 
#define MOTOR1_AIN2 25
#define STBY 27
#define PWMA 33
#define MOTOR2_BIN1 14
#define MOTOR2_BIN2 12
#define PWMB 13

TaskHandle_t tarefa1;
TaskHandle_t tarefa2;

const char* ssid     = "LABRA";
const char* password = "labra1234";

WiFiServer server(80);


void setup()
{
    Serial.begin(115200);
    
    pinMode(PWMA,OUTPUT);
    pinMode(MOTOR1_AIN1,OUTPUT);
    pinMode(MOTOR1_AIN2,OUTPUT);

    pinMode(PWMB,OUTPUT);
    pinMode(MOTOR2_BIN1,OUTPUT);
    pinMode(MOTOR2_BIN2,OUTPUT);
    pinMode(STBY,OUTPUT);      
    
    pinMode(LED_BUILTIN, OUTPUT); //led para visualização de conexão Wifi

    digitalWrite(STBY,LOW);  
    digitalWrite(MOTOR1_AIN1, LOW);
    digitalWrite(MOTOR1_AIN2, LOW);
    digitalWrite(MOTOR2_BIN1, LOW);
    digitalWrite(MOTOR2_BIN2, LOW);
    analogWrite(PWMA,0);
    analogWrite(PWMB,0);     
    
    delay(10);

    xTaskCreatePinnedToCore(controlePWM, "tarefa1", 10000, NULL, 0, &tarefa1, 0);    
    vTaskSuspend(tarefa1);
    // xTaskCreatePinnedToCore(controlePWM2, "tarefa2", 10000, NULL, 1, &tarefa2, 1);
    // vTaskSuspend(tarefa2);
    
    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    digitalWrite(LED_BUILTIN,HIGH);   //Conexão realizada Led acesso 
    
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Clique <a href=\"/ATIVAFRENTE\">Aqui</a> para acionar a Task1.<br>");
            client.print("Clique <a href=\"/DESATIVAFRENTE\">Aqui</a> para desacionar a Task1.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /ATIVAFRENTE")) {
          vTaskResume(tarefa1);
        }
        if (currentLine.endsWith("GET /DESATIVAFRENTE")) {
          vTaskSuspend(tarefa1);                
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void controlePWM(void *pvParameters){
    for(;;){
      
          digitalWrite(STBY,HIGH); 
          digitalWrite(MOTOR1_AIN1, LOW);
          digitalWrite(MOTOR1_AIN2, HIGH);
          digitalWrite(MOTOR2_BIN1, LOW);
          digitalWrite(MOTOR2_BIN2, HIGH);
          analogWrite(PWMA, 100);
          analogWrite(PWMB, 100);
      
        delay(30);
        }
    }

  



// void controlePWM2(void *pvParameters){
//     for(;;){
//       digitalWrite(STBY,HIGH); 
//     } 
// }
