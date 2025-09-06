#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "wifi.h"
#include "mqtt.h"



// MQTT Broker
const char* MQTT_BROKER = "XXXXXX";
const int MQTT_PORT = XXXXXX;
// const char* MQTT_USER = "";
// const char* MQTT_PASS = "";
const char* ID_CLIENT = "XXXXXX";
const char* TOPIC = "dispositivos/posto_enfermagem/ID_CLIENT";



WiFiClient espClient;
PubSubClient client(espClient);




void connectMQTT() {

  client.setServer(MQTT_BROKER, MQTT_PORT);

  if (client.connect(ID_CLIENT)) {
    Serial.println("Exito na conexão");
    Serial.printf("Cliente %s conectado ao Broker\n", ID_CLIENT);
    client.setCallback(callback);
    client.subscribe(TOPIC);

  } else {
    Serial.print("Falha ao conectar: ");
    Serial.println(client.state());
    Serial.println();
  }
}

//funcao publicar dados no topico(para testes)
void publishData() {
  // Criando o objeto JSON
  StaticJsonDocument<200> doc;
  doc["id"] = ID_CLIENT;
  doc["local"] = "Enfermaria";
  doc["room_number"] = 1;
  doc["timestamp"] = millis();
  doc["comando"] = "ligar";


  char payload[200];
  serializeJson(doc, payload);

  // Publicando o JSON no tópico
  client.publish(TOPIC, payload);
}



void callback(char* TOPIC, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no topico");
  Serial.println(TOPIC);
  Serial.print("Mensagem:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("------------------");
}

