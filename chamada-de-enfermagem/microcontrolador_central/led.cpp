#include "led.h"  // Inclui o header correspondente, que contém a definição do pino e protótipos das funções
#include "config.h"

// -----------------------------------------------------------------------------
// Variáveis globais
// -----------------------------------------------------------------------------

/**
 * @brief Armazena o momento (em ms) em que o LED foi alternado pela última vez.
 * 
 * Usado junto com a função millis() para controlar o intervalo de piscar o LED
 * sem bloquear o código com delays.
 */
unsigned long lastTimeLedTurnedOn = 0;

/**
 * @brief Intervalo em milissegundos para alternar o estado do LED.
 * 
 * Neste caso, definido como 2000 ms (2 segundos).
 */
constexpr unsigned long LED_INTERVAL_MS = 1000;


// -----------------------------------------------------------------------------
// Implementação das funções
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o LED configurando o pino como saída.
 * 
 * Deve ser chamada no setup() para preparar o hardware antes do uso.
 */
void ledInit() {
  pinMode(PIN_LED, OUTPUT);    // Configura pino do LED como saída
  digitalWrite(PIN_LED, LOW);  // Garante que o LED inicie apagado
}

/**
 * @brief Alterna o estado do LED em intervalos regulares.
 * 
 * Utiliza a função millis() para verificar se já se passou o tempo definido em
 * LED_INTERVAL_MS desde a última alteração de estado.
 * 
 * @param now - Tempo atual em milissegundos (retornado por millis()).
 */
void toggleLed() {

  unsigned long now = millis();
  // Verifica se o intervalo de tempo já passou
  if (now - lastTimeLedTurnedOn >= LED_INTERVAL_MS) {
    lastTimeLedTurnedOn = now;  // Atualiza o último momento da alternância

    // Inverte o estado atual do LED:
    // HIGH -> LOW ou LOW -> HIGH
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
  }
}

/**
 * @brief Desliga o LED forçando o pino para nível lógico LOW.
 */
void turnOffLed() {
  digitalWrite(PIN_LED, LOW);
}

/**
 * @brief Liga o LED forçando o pino para nível lógico HIGH.
 */
void turnOnLed() {
  digitalWrite(PIN_LED, HIGH);
}
