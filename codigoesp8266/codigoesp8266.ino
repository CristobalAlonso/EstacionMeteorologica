#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Movistar_Bastianuco 2,4GHZ";
const char* password = "Venus-Pandora5519";
const char* mqtt_server = "68.183.119.177"; // Este es el address en mqtt dash
const char* keyDevice = "st";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int pinLed1 = 16; //Conectarlo al D0
int pinLed2 = 5; //Conec
int pinLed3 = 4; //Conectarlo al D2
int pinLed4 = 0; //Conectarlo al D3
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("OK");
  }
void callback(char* topic, byte* payload, unsigned int length) {
 String topico = topic;
if(topico=="st/01")
{
    if (((char)payload[0] == '1')) { // 1 para prender
      digitalWrite(pinLed1,HIGH);
      Serial.println("Luz azul encendida = st/01");
  } else if (((char)payload[0] == '0')) { // 0 para apagar
      digitalWrite(pinLed1,LOW);
      Serial.println("Luz azul apagada = st/01");}
}
else if(topico=="st/02")
{
    if (((char)payload[0] == '1')) { // 1 para prender
      digitalWrite(pinLed2,HIGH);
      Serial.println("Luz roja encendida = st/02");
  } else if (((char)payload[0] == '0')) { // 0 para apagar
      digitalWrite(pinLed2,LOW);
      Serial.println("Luz roja apagada = st/02");}
}
else if(topico=="st/03")
{
    if (((char)payload[0] == '1')) { // 1 para prender
      digitalWrite(pinLed3,HIGH);
      Serial.println("Luz verde encendida = st/03");
  } else if (((char)payload[0] == '0')) { // 0 para apagar
      digitalWrite(pinLed3,LOW);
      Serial.println("Luz verde apagada = st/03");}
}
else if(topico=="st/04")
{
    if (((char)payload[0] == '1')) { // 1 para prender
      digitalWrite(pinLed4,HIGH);
      Serial.println("Luz amarilla encendida = st/04");
  } else if (((char)payload[0] == '0')) { // 0 para apagar
      digitalWrite(pinLed4,LOW);
      Serial.println("Luz amarilla apagada = st/04");}
}
}

void reconnect() {

  if (!client.connected()) {

    if (client.connect(keyDevice,"","")){//Servidor broker es el usuario y prueba12 la contraseña en mqtt dash
      Serial.println("connected");
      client.subscribe("st/#"); // Topico al momento de crear el boton
    } else {
      delay(5000);
    }
  }
}
void setup() {
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
  pinMode(pinLed3, OUTPUT);
  pinMode(pinLed4, OUTPUT);
  Serial.begin(9600);
  Serial.println("iniciando");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);   
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
