#include <WiFiClientSecure.h>   // Biblioteca para conexões seguras via TLS/SSL (necessária para MQTT com SSL/TLS).
#include "mqtt.h"               // Header com as declarações das funções e variáveis MQTT usadas no projeto.
#include "config.h"             // Arquivo de configuração com constantes (servidor, usuário, senha, tópicos, etc.).

// -----------------------------------------------------------------------------
// Objetos globais
// -----------------------------------------------------------------------------
WiFiClientSecure espClient;     // Cliente Wi-Fi seguro (usado pelo PubSubClient para comunicação MQTT).
PubSubClient client(espClient); // Cliente MQTT que usa o cliente Wi-Fi seguro como transporte.

// -----------------------------------------------------------------------------
// Variáveis de controle
// -----------------------------------------------------------------------------
static unsigned long lastAttempConnectMQTT = 0;         // Guarda o tempo da última tentativa de conexão com o broker.
static const unsigned long reconnectIntervalMQTT = 3000; // Intervalo (ms) entre tentativas de reconexão ao broker.

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
  espClient.setInsecure();              // Desabilita verificação de certificado SSL (não verifica autenticidade).
  client.setServer(MQTT_SERVER, 8883);  // Define o servidor MQTT e a porta (8883 = padrão para MQTTs).
  client.setCallback(callback);         // Registra a função callback para mensagens recebidas.
}

/**
 * @brief Verifica a conexão com o broker MQTT e reconecta se necessário.
 * 
 * A cada chamada, mantém a conexão ativa (client.loop()).
 * Se a conexão caiu, tenta reconectar dentro do intervalo definido.
 */
void checkMQTTConnected() {
  client.loop();                         // Mantém a comunicação ativa e processa mensagens recebidas.
  if (client.connected()) return;        // Se já está conectado, sai da função.

  unsigned long now = millis();          // Captura o tempo atual (ms desde o boot).

  // Só tenta reconectar se já passou o intervalo configurado
  if (now - lastAttempConnectMQTT >= reconnectIntervalMQTT){
    lastAttempConnectMQTT = now;         // Atualiza o tempo da última tentativa

    Serial.print("Tentando conectar ao MQTT... ");
    // Tenta conectar ao broker usando credenciais do config.h
    if (client.connect(MQTT_DEVICE_ID, MQTT_USER, MQTT_PASS)) {
      Serial.println("Conectado!");
      client.subscribe(MQTT_SUBSCRIPTION_TOPIC); // Inscreve-se no tópico para receber mensagens
    } else {
      Serial.print("Erro, rc=");
      Serial.println(client.state());    // Mostra o código de erro da conexão
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
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);

  Serial.print("Mensagem: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]); // Converte cada byte para caractere e imprime
  }
  Serial.println();
}

/**
 * @brief Publica um valor numérico (float) em um tópico MQTT.
 * 
 * O valor é formatado com duas casas decimais antes do envio.
 * 
 * @param id    - Sufixo do tópico (ex.: ID do dispositivo ou sensor).
 * @param value - Valor numérico a ser enviado.
 */
void publicReponseDivice(const char* id, float value) {
  char payload[10];
  snprintf(payload, sizeof(payload), "%.2f", value); // Converte float para string com 2 casas decimais.

  char topic[50];
  snprintf(topic, sizeof(topic), "%s/%s", MQTT_PUBLICATION_TOPIC, id); // Monta o tópico final (base + id).

  client.publish(topic, payload); // Publica a mensagem no broker MQTT.
}
