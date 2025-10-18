#include "config.h"
#include "mqtt.h"
#include "log.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "storage.h"
#include "config_storage.h"

/**
 * @file mqtt.cpp
 * @brief Implementação das funções relacionadas à comunicação MQTT.
 *
 * Este módulo gerencia:
 * - A configuração e a conexão com o broker MQTT via TLS/SSL;
 * - O envio de dados de sensores em formato JSON;
 * - O reenvio de dados armazenados localmente (em caso de falhas);
 * - O envio de mensagens de alerta.
 */


// -----------------------------------------------------------------------------
// Objetos MQTT
// -----------------------------------------------------------------------------

/// Cliente seguro para comunicação criptografada via TLS/SSL.
WiFiClientSecure espClient;

/// Cliente MQTT da biblioteca PubSubClient, utilizando o cliente seguro.
PubSubClient client(espClient);


// -----------------------------------------------------------------------------
// Variáveis de controle de reconexão e envio
// -----------------------------------------------------------------------------

static unsigned long lastAttempConnectMQTT = 0;           ///< Tempo da última tentativa de conexão ao broker MQTT.
static const unsigned long reconnectIntervalMQTT = 3000;  ///< Intervalo mínimo entre tentativas de reconexão (ms).

static unsigned long lastConnectionDataSent = 0;       ///< Tempo da última tentativa de envio de mensagem armazenada.
static const unsigned long resendIntervalMQTT = 3000;  ///< Intervalo mínimo entre reenvios de mensagens (ms).


// -----------------------------------------------------------------------------
// Funções de inicialização e conexão
// -----------------------------------------------------------------------------

/**
 * @brief Configura o cliente MQTT e inicializa a conexão segura.
 *
 * Define o servidor MQTT (endereço e porta) e desativa a verificação de certificado
 * para permitir comunicação segura mesmo sem validação completa do SSL/TLS.
 * 
 * Esta função deve ser chamada apenas uma vez, normalmente durante a inicialização
 * do sistema (por exemplo, no `setup()`).
 */
void setupMQTT() {
  espClient.setInsecure();  // Desativa a verificação do certificado TLS
  client.setServer(cfg.mqttServer.c_str(), 8883);
  log(LOG_DEBUG, "MQTT inicializado");
}


/**
 * @brief Verifica e mantém a conexão com o broker MQTT.
 *
 * - Executa `client.loop()` para processar pacotes MQTT pendentes.
 * - Se desconectado, tenta reconectar dentro do intervalo configurado.
 *
 * @return true  Se o cliente está conectado (ou reconectado com sucesso).
 * @return false Se ainda não foi possível restabelecer a conexão.
 */
bool checkMQTTConnected() {

  if (client.connected()) {
    client.loop();  // Mantém o link ativo e processa mensagens
    return true;    // Já está conectado
  }

  unsigned long now = millis();  // Obtém tempo atual

  // Verifica se já passou o intervalo mínimo entre tentativas
  if (now - lastAttempConnectMQTT >= reconnectIntervalMQTT) {
    lastAttempConnectMQTT = now;  // Atualiza timestamp da última tentativa

    log(LOG_DEBUG, "Tentando conectar ao MQTT...");

    // Tenta autenticar no broker com credenciais do arquivo de configuração
    if (client.connect(cfg.mqttDeviceId.c_str(), cfg.mqttUser.c_str(), cfg.mqttPass.c_str())) {
      log(LOG_INFO, "Conectado ao broker MQTT");
      return true;
    } else {
      log(LOG_ERROR, "Erro ao conectar com broker (rc=%d)", client.state());


      static uint8_t connection_drop_count = 0;

      connection_drop_count++;

      if (connection_drop_count >= 3) {
        resetMQTTClient();
        connection_drop_count = 0;
      }

      return false;
    }
  }

  return false;  // Intervalo ainda não atingido
}


// -----------------------------------------------------------------------------
// Funções de publicação
// -----------------------------------------------------------------------------

/**
 * @brief Publica dados de sensores (temperatura e umidade) em um tópico MQTT.
 *
 * O payload é gerado em formato JSON contendo:
 * - `Microcontrollerid` — Identificador do dispositivo (ESP);
 * - `temperature` — Valor de temperatura;
 * - `humidity` — Valor de umidade.
 *
 * Exemplo de JSON publicado:
 * ```json
 * {
 *   "Microcontrollerid": "esp32-horta01",
 *   "temperature": 26.4,
 *   "humidity": 58.3
 * }
 * ```
 *
 * @param temperature Valor da temperatura em °C.
 * @param humidity    Valor da umidade relativa (%).
 * @return true  Se os dados foram enviados com sucesso.
 * @return false Se ocorreu falha na publicação.
 */
bool publishSensorData(float temperature, float humidity) {
  StaticJsonDocument<128> doc;
  doc["Microcontrollerid"] = cfg.mqttDeviceId.c_str();
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;

  char buffer[128];
  serializeJson(doc, buffer);

  if (!client.publish(cfg.mqttTopicData.c_str(), buffer)) {
    log(LOG_WARN, "Falha ao enviar dados ao broker MQTT");
    return false;
  }

  return true;
}


/**
 * @brief Reenvia dados armazenados localmente (em caso de falha anterior).
 *
 * - Verifica se há dados salvos no sistema de armazenamento (LittleFS);
 * - Envia um registro por vez, respeitando o intervalo mínimo entre reenvios;
 * - Caso o envio seja bem-sucedido, remove o registro do armazenamento.
 */
void resendMqttData() {
  if (!hasDataStorage()) return;  // Nenhum dado armazenado

  unsigned long now = millis();

  // Respeita o intervalo mínimo entre reenvios
  if (now - lastConnectionDataSent >= resendIntervalMQTT) {
    lastConnectionDataSent = now;

    ObjectStorage obj = getObjectStorage();  // Obtém o primeiro objeto salvo
    if (obj.valid) {
      if (publishSensorData(obj.temperature, obj.humidity)) {
        deleteOneMessage();  // Remove a mensagem já reenviada
      }
    }
  }
}


/**
 * @brief Publica uma mensagem de alerta no broker MQTT.
 *
 * O payload é gerado em formato JSON contendo:
 * - `Microcontrollerid` — Identificador do dispositivo;
 * - `alert` — Texto descritivo do alerta.
 *
 * Exemplo de JSON:
 * ```json
 * {
 *   "Microcontrollerid": "esp32-horta01",
 *   "alert": "Nível de umidade abaixo do mínimo"
 * }
 * ```
 *
 * @param alert Mensagem de alerta a ser enviada.
 */
void publishAlert(const char* alert) {
  StaticJsonDocument<96> doc;
  doc["Microcontrollerid"] = cfg.mqttDeviceId.c_str();
  doc["alert"] = alert;

  char buffer[96];
  serializeJson(doc, buffer);
  client.publish(cfg.mqttTopicAlert.c_str(), buffer);
}


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
 * - Desativa a verificação de certificado TLS (`setInsecure`);
 * - Redefine o servidor MQTT e a porta segura;
 * - Reassocia o cliente TLS ao cliente MQTT.
 *
 * Essa rotina evita a necessidade de reiniciar o microcontrolador.
 */
void resetMQTTClient() {
  log(LOG_WARN, "Reinicializando cliente MQTT e TLS...");

  espClient = WiFiClientSecure();  // recria o cliente seguro
  espClient.setInsecure();         // desativa verificação de certificado
  client.setServer(cfg.mqttServer.c_str(), 8883);
  client.setClient(espClient);

  log(LOG_DEBUG, "Cliente MQTT reconfigurado");
}
