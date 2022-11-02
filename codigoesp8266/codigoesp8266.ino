#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <MQ135.h>

//conexion internet wifi  y server
const char* ssid = "Movistar_Bastianuco 2,4GHZ"; //user wifi
const char* password = "Venus-Pandora5519"; //contraseña wifi
const char* mqtt_server = "68.183.119.177"; // Este es el address en mqtt dash
//conexion internet wifi y server

//buscar????
const char* keyDevice = "st"; //

//variables de esp
WiFiClient espClient;
PubSubClient client(espClient);

//varuables numericas
long lastMsg = 0;
int value = 0;

//variables conexion pin
MQ135 gasSensor = MQ135 (A0); //gas sensor se necesita la libreria mq135
int pinLed1 = 16; //Conecta el pin 16 un led
//variables conexion pin

/*
   printDataMQ135()
   publica los datos del mq135 castiando de float to const char*
   Sensor importante proyecto alemdra
   param    -> void
   return   -> void
*/
void printDataMQ135()
{
  char temperature[10];
  float h = analogRead(A0);
  float mq175 = h / 1023 * 100;
  client.publish("test/msg", String(mq175).c_str() );
}

/*
   delaySec(int x)
   Funcion que detiene el codigo en los segundos que sean ingresados a travez de los parametros
   param    -> int
   return   -> void
*/
void delaySec(int x)
{
  int minDel = x*1000;
  delay(minDel);
}

//Conexion wifi
void setup_wifi() \
{
  delay(10);
  WiFi.begin(ssid, password); //ingresa credenciales
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");//mientras realiza la conexion imprime puntos de espera
    delay(500);//espera 500 microsegundos
  }
  Serial.println("OK");
}


void callback(char* topic, byte* payload, unsigned int length)
{
  //topico cual conecta con mqtt
  String topico = topic;

  topico = "test/msg";
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }


  client.subscribe(topic);


  if (topico == "st/01")
  {
    if (((char)payload[0] == '1')) // 1 para prender
    {
      digitalWrite(pinLed1, HIGH);
      Serial.println("Luz azul encendida = st/01");
    }
    else if (((char)payload[0] == '0')) // 0 para apagar
    {
      digitalWrite(pinLed1, LOW);
      Serial.println("Luz azul apagada = st/01");
    }
  }
}

//en caso de que se caiga la conexion o para reconectar constantemente
void reconnect() {
  if (!client.connected())
  {
    if (client.connect(keyDevice, "", "")) //Servidor broker es el usuario y prueba12 la contraseña en mqtt dash
    {
      Serial.println("connected");
      client.subscribe("st/#"); // Topico al momento de crear el boton
    }
    else
    {
      delay(5000);
    }
  }
}

//configuraciones de la placa
void setup() {
  pinMode(pinLed1, OUTPUT);
  Serial.begin(9600);
  Serial.println("iniciando");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

//configuraciones de la palca en bucle para conexion de mqtt
void loop()
{
  printDataMQ135();
  //delaySec(30);
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
