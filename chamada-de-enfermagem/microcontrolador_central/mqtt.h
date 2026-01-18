#ifndef MQTT_HELPER_H   // Diretiva de pré-processador para evitar inclusões múltiplas do mesmo header
#define MQTT_HELPER_H   // Define a constante de guarda para o header

#include <PubSubClient.h>   // Biblioteca que implementa o cliente MQTT (conexão, publish, subscribe, etc.)

// -----------------------------------------------------------------------------
// Declarações globais
// -----------------------------------------------------------------------------

extern const char* calledToBeErased;

extern bool hasOKMessage;

/**
 * @brief Objeto global do cliente MQTT.
 * 
 * É declarado aqui como "extern" porque a definição real está no arquivo .cpp.
 * Isso permite que qualquer arquivo que inclua este header possa acessar o objeto.
 */
extern PubSubClient client;

// -----------------------------------------------------------------------------
// Assinaturas das funções
// -----------------------------------------------------------------------------

/**
 * @brief Configura a conexão com o broker MQTT.
 * 
 * Define servidor, porta e função de callback responsável por tratar mensagens recebidas.
 * Deve ser chamada na inicialização do sistema.
 */
void setupMQTT();

/**
 * @brief Verifica e mantém a conexão com o broker MQTT.
 * 
 * Se a conexão cair, tenta reconectar automaticamente dentro do intervalo definido.
 * Deve ser chamada periodicamente (ex.: dentro do loop principal).
 */
void checkMQTTConnected();

/**
 * @brief Função callback chamada quando uma mensagem MQTT é recebida.
 * 
 * @param topic   - Tópico no qual a mensagem foi publicada.
 * @param payload - Ponteiro para os dados recebidos.
 * @param length  - Tamanho da mensagem em bytes.
 */
void callback(char* topic, byte* payload, unsigned int length);

/**
 * @brief Publica um valor numérico formatado em um tópico MQTT.
 * 
 * O valor é enviado como string com duas casas decimais.
 * O tópico final é montado dinamicamente com base em uma constante + o id fornecido.
 * 
 * @param id    - Identificador do dispositivo ou sensor (sufixo do tópico).
 * @param value - Valor numérico (float) a ser enviado.
 */
bool publicReponseDivice(const char* deviceId, const char* topic, const char* message, bool retained = false);

#endif // MQTT_HELPER_H
