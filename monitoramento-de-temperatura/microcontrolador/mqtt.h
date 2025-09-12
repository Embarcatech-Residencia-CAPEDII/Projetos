#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

#include <PubSubClient.h>

/**
 * @file mqtt.h
 * @brief Declarações de funções e variáveis globais para comunicação MQTT.
 *
 * Este módulo define a interface pública para configuração,
 * manutenção de conexão e publicação de mensagens em tópicos MQTT.
 */

// -----------------------------------------------------------------------------
// Variáveis globais
// -----------------------------------------------------------------------------

/// Cliente MQTT global utilizado para comunicação com o broker.
/// É configurado e inicializado em `mqtt.cpp`.
extern PubSubClient client;

// -----------------------------------------------------------------------------
// Funções públicas
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa a configuração para conexão MQTT.
 *
 * Define parâmetros básicos como broker, porta e credenciais.
 * Deve ser chamada durante a fase de inicialização do sistema.
 */
void setupMQTT();

/**
 * @brief Garante que a conexão MQTT esteja ativa.
 *
 * Se a conexão for perdida, tenta reconectar automaticamente
 * utilizando as credenciais configuradas.
 */
void checkMQTTConnected();

/**
 * @brief Publica dados de sensores em formato JSON.
 *
 * O payload inclui:
 * - Identificação do dispositivo
 * - Temperatura
 * - Umidade
 *
 * @param temperature Valor da temperatura a ser enviado.
 * @param humidity    Valor da umidade a ser enviado.
 */
void publishSensorData(float temperature, float humidity);

/**
 * @brief Publica uma mensagem de alerta em formato JSON.
 *
 * O payload inclui:
 * - Identificação do dispositivo
 * - Mensagem de alerta
 *
 * @param alert String com a mensagem de alerta a ser enviada.
 */
void publishAlert(const char* alert);

#endif
