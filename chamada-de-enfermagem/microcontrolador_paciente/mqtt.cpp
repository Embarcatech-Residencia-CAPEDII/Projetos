#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "wifi.h"
#include "mqtt.h"
#include "led.h"

// Variáveis de controle
static unsigned long lastAttempConnectMQTT = 0;           // Guarda o tempo da última tentativa de conexão com o broker.
static const unsigned long reconnectIntervalMQTT = 3000;  // Intervalo (ms) entre tentativas de reconexão ao broker.
extern bool buttonBlocked;   // Indica se o botão está bloqueado para novos acionamentos (true = bloqueado, false = liberado)
extern bool confirmMsg; // Indica se o dispositivo recebeu confirmação via callback (status: "ok")

//MQTT Broker 
const char* MQTT_BROKER = "XXXXXX";
const int MQTT_PORT = 0;
const char* MQTT_USER = "";
const char* MQTT_PASS = "";
const char* ID_CLIENT = "{id-dispositivo}";

const char* TOPIC_PUBLISH = "dispositivos/posto_enfermaria";
const char* TOPIC_SUBSCRIBE = "dispositivos/enfermaria/{id-dispositivo}";

const char* PUBLISH_MSG_CONFIRM = "dispositivo/confirmacao/posto_enfermaria";
const char* TOPIC_SUBSCRIBE_CONFIRM = "dispositivo/confirmacao/{id-dispositivo}";




WiFiClient espClient;
PubSubClient client(espClient);


void setupMQTT() {
  
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);
}


void connectMQTT() {

  unsigned long now = millis();  // Captura o tempo atual (ms desde o boot).
  // Só tenta reconectar se já passou o intervalo configurado
  if (now - lastAttempConnectMQTT >= reconnectIntervalMQTT) {
    lastAttempConnectMQTT = now;  // Atualiza o tempo da última tentativa

    Serial.println("Tentando conectar ao MQTT");
    // Tenta conectar ao broker
    if (client.connect(ID_CLIENT, MQTT_USER, MQTT_PASS)) {
      Serial.println("Exito na conexão");
      client.subscribe(TOPIC_SUBSCRIBE);
      client.subscribe(TOPIC_SUBSCRIBE_CONFIRM);  // Inscreve-se no tópico para receber mensagens
    } else {
      Serial.print("Falha ao conectar: ");
      Serial.println(client.state());
      Serial.println();  // Mostra o código de erro da conexão
    }
  }
}


// Criando o objeto JSON
const char* createJsonPayload() {

  StaticJsonDocument<200> doc;
  doc["id"] = ID_CLIENT;
  doc["estado"] = "emergencia";
  doc["mensagem"] = "solicitar atendimento";
  doc["room_number"] = 1;
  doc["local"] = "enfermaria";
  doc["comando"] = "ligar";

  static char payload[200];
  serializeJson(doc, payload);
  return payload;
}


// Cria o objeto JSON de msg de confirmacao
const char* createJsonConfirmPayload() {

  StaticJsonDocument<64> doc;
  doc["id"] = ID_CLIENT;
  doc["status"] = "ok";

  static char payload[64];
  serializeJson(doc, payload);
  return payload;
}



void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("Erro ao parsear JSON: ");
    Serial.println(error.c_str());
    return;
  }


  if (String(topic) == TOPIC_SUBSCRIBE) {

    if (doc.containsKey("comando") && !doc["comando"].isNull()) {
      const char* comando = doc["comando"];
      Serial.println("Comando: " + String(comando));

      if (String(comando) == "desligar") {
        desligarLed();
        if(client.publish(PUBLISH_MSG_CONFIRM, createJsonConfirmPayload(), false)){
          Serial.println("Enviado confirmacao de finalizado!");
        }
        buttonBlocked = false;
        confirmMsg = true;
        Serial.println("Solicitação finalizada. Botão liberado!");
      }
    } else {
      Serial.println("Comando ausente ou nulo");
    }

  }

  else if (String(topic) == TOPIC_SUBSCRIBE_CONFIRM) {

    if (doc.containsKey("status") && !doc["status"].isNull()) {
      const char* status = doc["status"];
      Serial.println("Status: " + String(status));

      if (String(status) == "ok") {
        Serial.println("Solicitação recebida pelo enfermeiro");
        ligarLed();
        buttonBlocked = true;
        confirmMsg = true;
      }
    } else {
      Serial.println("Comando ausente ou nulo");
    }
  }
}
