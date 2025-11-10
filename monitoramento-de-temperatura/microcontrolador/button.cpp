/**
 * @file button.cpp
 * @brief Implementação das rotinas de controle e interrupção de botões físicos.
 *
 * Este módulo lida com o gerenciamento de dois botões:
 * - Um botão de **mute** (curto clique)
 * - Um botão de **long press** (pressionamento longo)
 *
 * Inclui tratamento de debounce, alternância lógica (toggle), 
 * e detecção de pressões longas baseadas em tempo.
 */

#include "c_types.h"
#include "button.h"
#include "log.h"
#include "config.h"

// -----------------------------------------------------------------------------
// Variáveis globais
// -----------------------------------------------------------------------------

/// Marca o tempo em que o botão começou a ser pressionado (usado para long press)
volatile unsigned long buttonPressStartTime = 0;

/// Indica se ocorreu uma pressão longa no botão (pressionado > 15s)
volatile bool flagLongPress = false;

volatile bool flagShortPress = false;

/// Indica se o botão está atualmente pressionado
volatile bool isButtonPressed = false;

/**
 * @brief Flag que indica se o botão "mute" foi pressionado.
 * 
 * É `volatile` porque pode ser modificada dentro de uma ISR (rotina de interrupção),
 * garantindo leitura correta mesmo em contextos concorrentes.
 */
volatile bool flagMutePressed = false;


bool isLCDturnOn = false;

/**
 * @brief Estado lógico atual do botão de mute.
 * 
 * Alterna (toggle) entre `true` e `false` a cada clique válido.
 */
bool isMuted = false;


// -----------------------------------------------------------------------------
// Rotinas de interrupção (ISR)
// -----------------------------------------------------------------------------

/**
 * @brief ISR do botão de mute.
 *
 * Chamado automaticamente a cada borda de subida (RISING) no pino do botão.
 * Implementa um **debounce por software** para evitar múltiplas detecções
 * de um mesmo clique físico.
 *
 * Se o intervalo desde a última interrupção for maior que 500 ms, a flag
 * `flagMutePressed` é ativada.
 */
void IRAM_ATTR muteButtonISR() {
  static unsigned long lastInterruptTime = 0;  ///< Marca o instante da última interrupção
  unsigned long currentTime = millis();

  if (currentTime - lastInterruptTime > 500) {
    flagMutePressed = true;  // Marca que o botão foi pressionado
  }

  lastInterruptTime = currentTime;
}

/**
 * @brief ISR para detecção de pressão longa.
 *
 * Detecta quanto tempo o botão permanece pressionado.
 * Se for pressionado por mais de 15 segundos, ativa a flag `flagLongPress`.
 */
void IRAM_ATTR longPressButtonISR() {
  bool readButton = digitalRead(PIN_BUTTON_LONG);

  if (readButton == LOW && !isButtonPressed) {
    // Início da pressão
    buttonPressStartTime = millis();
    isButtonPressed = true;

  } else if (readButton == HIGH && isButtonPressed) {
    // Soltou o botão
    unsigned long now = millis();
    if (now - buttonPressStartTime >= 15 * 1000) {
      flagLongPress = !flagLongPress;
    } else if (now - buttonPressStartTime < 15 * 1000 && now - buttonPressStartTime >= 100) {
      flagShortPress = true;
      
    }
    isButtonPressed = false;
  }
}


// -----------------------------------------------------------------------------
// Funções públicas
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o pino do botão.
 *
 * Configura o pino como entrada com **pull-up interno**,
 * preparando-o para ser utilizado com interrupções.
 *
 * @param pin Pino digital conectado ao botão.
 */
void buttonInit(uint8_t pin) {
  pinMode(pin, INPUT_PULLUP);
}

/**
 * @brief Habilita a interrupção por borda de subida (RISING).
 *
 * Ativa a ISR `muteButtonISR()` para capturar cliques rápidos.
 *
 * @param pin Pino do botão configurado.
 */
void enableButtonInterruptRising(uint8_t pin) {
  attachInterrupt(digitalPinToInterrupt(pin), muteButtonISR, RISING);
}

/**
 * @brief Desabilita a interrupção do botão de mute.
 *
 * Impede que novas interrupções de clique sejam processadas.
 */
void disableButtonInterruptRising(uint8_t pin) {
  detachInterrupt(digitalPinToInterrupt(pin));
}

/**
 * @brief Habilita a interrupção por mudança de estado (CHANGE).
 *
 * Ativa a ISR `longPressButtonISR()` para monitorar o tempo de pressão do botão.
 *
 * @param pin Pino do botão de long press.
 */
void enableButtonInterruptChange(uint8_t pin) {
  attachInterrupt(digitalPinToInterrupt(pin), longPressButtonISR, CHANGE);
}

/**
 * @brief Desabilita a interrupção de long press.
 */
void disableButtonInterruptChange(uint8_t pin) {
  detachInterrupt(digitalPinToInterrupt(pin));
}

/**
 * @brief Verifica se o botão foi pressionado.
 *
 * Se a flag de clique estiver ativa, ela é limpa e retorna `true`.
 *
 * @param buttonPressed Ponteiro para a flag de estado do botão.
 * @return `true` se o botão foi pressionado.
 * @return `false` caso contrário.
 */
bool checkButtonPressed(volatile bool *buttonPressed) {
  if (*buttonPressed) {
    *buttonPressed = false;
    return true;
  }
  return false;
}

/**
 * @brief Retorna o estado lógico do botão de mute.
 *
 * Alterna o valor de `isMuted` a cada clique detectado.
 *
 * @return `true` se o sistema está mutado.
 * @return `false` caso contrário.
 */
bool wasMuted() {
  if (checkButtonPressed(&flagMutePressed)) {
    isMuted = !isMuted;
  }
  return isMuted;
}

/**
 * @brief Retorna o estado da flag de pressão longa.
 *
 * @return `true` se ocorreu uma pressão longa (> 15s).
 * @return `false` caso contrário.
 */
bool wasButtonLongPressed() {
  return flagLongPress;
}


bool wasButtonShortPress(){
  if (flagShortPress == true){
    flagShortPress = false;
    return true;
  }
  return false;
}

/**
 * @brief Reseta o estado lógico do botão de mute.
 *
 * Útil em rotinas de inicialização ou reinicialização do sistema.
 */
void resetButtonState() {
  isMuted = false;
}

