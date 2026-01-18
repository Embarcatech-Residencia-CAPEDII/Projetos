#include <WiFiClientSecure.h>    // Biblioteca para conexões seguras via TLS/SSL (necessária para MQTT com SSL/TLS).
#include "mqtt.h"                // Header com as declarações das funções e variáveis MQTT usadas no projeto.
#include "config.h"              // Arquivo de configuração com constantes (servidor, usuário, senha, tópicos, etc.).
#include "jsonDataProcessing.h"  // Header com a declaração da função que processa os dados JSON recebido do MQTT
#include "log.h"
#include "buzzer.h"
#include "config_storage.h"

// -----------------------------------------------------------------------------
// Objetos globais
// -----------------------------------------------------------------------------
WiFiClient espClient;            // Cliente Wi-Fi seguro (usado pelo PubSubClient para comunicação MQTT).
PubSubClient client(espClient);  // Cliente MQTT que usa o cliente Wi-Fi seguro como transporte.

// -----------------------------------------------------------------------------
// Variáveis de controle
// -----------------------------------------------------------------------------
static unsigned long lastAttempConnectMQTT = 0;           // Guarda o tempo da última tentativa de conexão com o broker.
static const unsigned long reconnectIntervalMQTT = 3000;  // Intervalo (ms) entre tentativas de reconexão ao broker.

bool hasOKMessage = false;

const char* calledToBeErased;



// -----------------------------------------------------------------------------
// Funções auxiliares
// -----------------------------------------------------------------------------

/**
 * @brief Reconfigura o cliente MQTT e o cliente TLS (WiFiClientSecure).
 *
 * Essa função é chamada quando o cliente MQTT falha repetidamente ao tentar
 * reconectar ao broker. Em situações assim, o cliente TLS pode ficar em um
 * estado inválido, bloqueando novas conexões. O procedimento abaixo recria
 * ambos os clientes para restaurar a comunicação.
 *
 * Operações executadas:
 * - Reinstancia o objeto `WiFiClientSecure` (limpa conexões antigas);
 * - Redefine o servidor MQTT e a porta segura;
 * - Reassocia o cliente TLS ao cliente MQTT.
 *
 * Essa rotina evita a necessidade de reiniciar o microcontrolador.
 */
void resetMQTTClient() {
  log(LOG_WARN, "Reinicializando cliente MQTT e TLS...");

  espClient = WiFiClient();  // recria o cliente seguro
  client.setServer(cfg.mqttServer.c_str(), cfg.mqttPort);
  client.setClient(espClient);

  log(LOG_DEBUG, "Cliente MQTT reconfigurado");
}



// -----------------------------------------------------------------------------
// Funções principais
// -----------------------------------------------------------------------------

/**
 * @brief Configura a conexão com o servidor MQTT.
 * 
 * Define parâmetros básicos como servidor, porta e função de callback
 * que será chamada ao receber mensagens.
 */
void setupMQTT() {
  // espClient.setInsecure();              // Desabilita verificação de certificado SSL (não verifica autenticidade).
  client.setServer(cfg.mqttServer.c_str(), cfg.mqttPort);  // Define o servidor MQTT e a porta (8883 = padrão para MQTTs).
  client.setCallback(callback);                            // Registra a função callback para mensagens recebidas.
}


/**
 * @brief Verifica a conexão com o broker MQTT e reconecta se necessário.
 * 
 * A cada chamada, mantém a conexão ativa (client.loop()).
 * Se a conexão caiu, tenta reconectar dentro do intervalo definido.
 */
void checkMQTTConnected() {
  client.loop();                   // Mantém a comunicação ativa e processa mensagens recebidas.
  if (client.connected()) return;  // Se já está conectado, sai da função.

  unsigned long now = millis();  // Captura o tempo atual (ms desde o boot).

  // Só tenta reconectar se já passou o intervalo configurado
  if (now - lastAttempConnectMQTT >= reconnectIntervalMQTT) {
    lastAttempConnectMQTT = now;  // Atualiza o tempo da última tentativa

    log(LOG_INFO, "Tentando conectar ao MQTT");
    // Tenta conectar ao broker usando credenciais do config.h
    if (client.connect(cfg.mqttDeviceId.c_str(), cfg.mqttUser.c_str(), cfg.mqttPass.c_str())) {
      log(LOG_INFO, "Conectado ao broker!");
      char fullTopic[50];
      snprintf(fullTopic, sizeof(fullTopic), "%s/%s", MQTT_SUBSCRIPTION_TOPIC, "#");
      client.subscribe(fullTopic);  // Inscreve-se no tópico para receber mensagens
      client.subscribe(MQTT_SUB_CONFIRMATION_TOPIC);

    } else {
      log(LOG_ERROR, "Falha na conexão com mqtt, rc= %d", client.state());  // Mostra o código de erro da conexão

      static uint8_t connection_drop_count = 0;

      connection_drop_count++;

      if (connection_drop_count >= 3) {
        resetMQTTClient();
        connection_drop_count = 0;
      }
    }
  }
}

/**
 * @brief Função callback chamada automaticamente quando uma mensagem MQTT é recebida.
 * 
 * @param topic   - Tópico no qual a mensagem foi publicada.
 * @param payload - Ponteiro para os dados recebidos.
 * @param length  - Tamanho da mensagem.
 */
void callback(char* topic, byte* payload, unsigned int length) {
  log(LOG_DEBUG, "Mensagem recebida no tópico: %s", topic);


  if (strcmp(topic, MQTT_SUB_CONFIRMATION_TOPIC) == 0) {
    // Serial.println("recebeu ok");
    calledToBeErased = getPayloadID(payload, length);
    log(LOG_INFO,"recebeu ok");
    hasOKMessage = true;
  }

  else if (strncmp(topic, MQTT_SUBSCRIPTION_TOPIC, strlen(MQTT_SUBSCRIPTION_TOPIC)) == 0) {
    // Processa os dados Json recebidos
    if (length == 0){
      return;
    }

    else if (processing_json_MQTT(payload, length)) {

      char buffer[50];
      publicReponseDivice(
        getPayloadID(payload, length),
        MQTT_PUB_CONFIRMATION_TOPIC,
        creteJsonPayloadConfirmationMessage(buffer, sizeof(buffer)),
        true
        );

      enableSoundAlert();
    }
  }
}

/**
 * @brief Publica um valor numérico (float) em um tópico MQTT.
 * 
 * O valor é formatado com duas casas decimais antes do envio.
 * 
 * @param id    - Sufixo do tópico (ex.: ID do dispositivo ou sensor).
 * @param value - Valor numérico a ser enviado.
 */
bool publicReponseDivice(const char* deviceId, const char* topic, const char* message, bool retained) {

  char fullTopic[50];
  snprintf(fullTopic, sizeof(fullTopic), "%s/%s", topic, deviceId);  // Monta o tópico final (base + id).


  // char buffer[256];
  if (!client.publish(fullTopic, message,retained)) {
    log(LOG_WARN, "Falha ao enviar dados ao broker MQTT");
    return false;
  }

  return true;
}
