#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

//definindo pinos de controle 
#define MOTOR1_AIN1 26 
#define MOTOR1_AIN2 25
#define STBY 27
#define PWMA 33
#define MOTOR2_BIN1 14
#define MOTOR2_BIN2 12
#define PWMB 13

// Variaveis PWM 
int MTA = 0;
int MTB = 0;

// Configurando nome e senha da rede Esp32
const char *ssid = "ESP32";
const char *password = "lab";

//Configurando porta de acesso entre servidor e cliente
WiFiServer server(80);

void setup() {
  //Configurando pinos como saída
  pinMode(PWMA,OUTPUT);
  pinMode(MOTOR1_AIN1,OUTPUT);
  pinMode(MOTOR1_AIN2,OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(MOTOR2_BIN1,OUTPUT);
  pinMode(MOTOR2_BIN2,OUTPUT);
  pinMode(STBY,OUTPUT);

  //Inicializar monitor serial
  Serial.begin(115200);
  Serial.println();
  Serial.println("configurando...");

  //Rede
  WiFi.softAP(ssid, password); //Gerando rede propria
  IPAddress myIP = WiFi.softAPIP(); //Coletando endereço na rede
  Serial.print("O Endereço IP Da Rede : ");
  Serial.println(myIP);
  
  //inicia o webserver
  server.begin();
  Serial.println("Servidor Iniciado!!");  
  
  //Colocando todos os pinos (menos ponte H) em nivel logico baixo/rodas livres
  digitalWrite(STBY,HIGH);  
  digitalWrite(MOTOR1_AIN1, LOW);
  digitalWrite(MOTOR1_AIN2, LOW);
  digitalWrite(MOTOR2_BIN1, LOW);
  digitalWrite(MOTOR2_BIN2, LOW);
  analogWrite(PWMA,MTA);
  analogWrite(PWMB,MTB);

}

void loop() {
  
WiFiClient client = server.available(); // verifica e armazena se tem clientes conectados na rede

if (client) { // se você conseguir um cliente,
Serial.println("Novo Cliente."); // imprime uma mensagem pela porta serial
String currentLine = ""; // faz uma String para manter os dados recebidos do cliente
while (client.connected()) { // loop enquanto o cliente está conectado
if (client.available()) { // se houver bytes para ler do cliente,
char c = client.read(); // leia um byte e depois
Serial.write(c); // imprima o monitor serial
if (c == '\n') { // se o byte for um caractere de nova linha


/* se a linha atual estiver em branco, você terá dois caracteres de nova linha em uma linha.
esse é o fim da solicitação HTTP do cliente, então envie uma resposta: */
if (currentLine.length() == 0) {
/* Os cabeçalhos HTTP sempre começam com um código de resposta (por exemplo, HTTP / 1.1 200 OK)
e um tipo de conteúdo para que o cliente saiba o que está por vir e, em seguida, uma linha em branco: */

client.println("HTTP/1.1 200 OK");
client.println  ("Content-type:text/html");
client.println();

client.print("<html>");
client.print("<head><title> ""Controle de Carrinho por Wi-Fi"" </title></head>");
client.print("<body>");

// configuração de estilo do site
client.print("<style type=\"text/css\"> a{margin:600px 600px; background: #228B22; color: #ffffff; text-decoration: none; padding: 0.5% 5% 0.5%; border-radius: 5px; font-size: 2.0em;}</style>");

//---------------Botão Frente para a página WEB----------------------
client.print("<a href=\"/FRENTE\">Frente </a> </br></br></br>");

//---------------Botão Voltar para a página WEB----------------------
client.print("<a href=\"/VOLTAR\">Voltar </a> </br></br></br>");

//---------------Botão Esquerda para a página WEB----------------------
client.print("<a href=\"/ESQUERDA\">Esquerda</a> </br></br></br>");

//---------------Botão Direita para a página WEB----------------------
client.print("<a href=\"/DIREITA\">Direita </a> </br></br></br>");

//---------------Botão Parar para a página WEB----------------------
client.print("<a href=\"/PARAR\">Parar </a> </br></br></br>");

client.print("</html>");
client.print("</body>");

// A resposta HTTP termina com outra linha em branco:
client.println();
// sair do loop while:
break;
} else { // se você tiver uma nova linha, então limpe a linha atual:
currentLine = "";
}
} else if (c != '\r') { // se você tiver qualquer outra coisa além de um caractere de retorno de linha,
currentLine += c; // adicioná-lo ao final da linha atual
}

// Verifica se o pedido do cliente foi para atualizar algum dos reles (GET /T)

if (currentLine.endsWith("GET /FRENTE")) {
  
digitalWrite(MOTOR1_AIN1, LOW);
digitalWrite(MOTOR1_AIN2, HIGH);
digitalWrite(MOTOR2_BIN1, LOW);
digitalWrite(MOTOR2_BIN2, HIGH);

// definir as velocidades dos motores
MTA = 60;
MTB = 60;

analogWrite(PWMA,MTA);
analogWrite(PWMB,MTB);;

Serial.println("Frente");

}
if (currentLine.endsWith("GET /VOLTAR")) {
// girar os motores para trás
digitalWrite(MOTOR1_AIN1, HIGH);
digitalWrite(MOTOR1_AIN2, LOW);
digitalWrite(MOTOR2_BIN1, HIGH);
digitalWrite(MOTOR2_BIN2, LOW);

// definir as velocidades dos motores
MTA = 60;
MTB = 60;

analogWrite(PWMA,MTA);
analogWrite(PWMB,MTB);


Serial.println("Voltar");

}
if (currentLine.endsWith("GET /DIREITA")) {

// girar os motores para trás
digitalWrite(MOTOR1_AIN1, HIGH);
digitalWrite(MOTOR1_AIN2, LOW);
digitalWrite(MOTOR2_BIN1, LOW);
digitalWrite(MOTOR2_BIN2, HIGH);

// definir as velocidades dos motores
MTA = 60;
MTB = 60;

analogWrite(PWMA,MTA);
analogWrite(PWMB,MTB);


Serial.println("Direita");

}

if (currentLine.endsWith("GET /ESQUERDA")) {
  
// girar os motores para esquerda
digitalWrite(MOTOR1_AIN1, LOW);
digitalWrite(MOTOR1_AIN2, HIGH);
digitalWrite(MOTOR2_BIN1, HIGH);
digitalWrite(MOTOR2_BIN2, LOW);

// definir as velocidades dos motores
MTA = 60;
MTB = 60;

analogWrite(PWMA,MTA);
analogWrite(PWMB,MTB);


}

if (currentLine.endsWith("GET /PARAR")) {
  
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
}
// termina a conexão com o cliente
client.stop();
//Serial.println("Cliente desconectado.");

}
