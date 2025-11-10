#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

/**
 * @file button.h
 * @brief Declarações de funções e variáveis para o controle de botões com interrupção.
 *
 * Este módulo oferece uma interface para o gerenciamento de dois tipos de botões:
 * - Um botão de **clique curto** (mute)
 * - Um botão de **pressão longa** (long press)
 *
 * O tratamento de debounce e lógica de alternância (toggle) são implementados em `button.cpp`.
 * As rotinas são compatíveis com microcontroladores baseados em Arduino/ESP.
 */

// -----------------------------------------------------------------------------
// Funções de inicialização e controle
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o pino do botão.
 *
 * Configura o pino especificado como **entrada com pull-up interno**,
 * preparando-o para ser utilizado com interrupções.
 *
 * @param pin Pino digital onde o botão está conectado.
 */
void buttonInit(uint8_t pin);

/**
 * @brief Habilita a interrupção do botão de clique curto (mute).
 *
 * A rotina de interrupção (`muteButtonISR`) será chamada automaticamente
 * a cada evento de borda de subida (**RISING**) no pino configurado.
 *
 * @param pin Pino do botão de mute.
 */
void enableButtonInterruptRising(uint8_t pin);

/**
 * @brief Desabilita a interrupção do botão de clique curto (mute).
 *
 * Após esta chamada, o botão não gerará mais interrupções
 * até que `enableButtonInterruptRising()` seja reativado.
 *
 * @param pin Pino do botão de mute.
 */
void disableButtonInterruptRising(uint8_t pin);

/**
 * @brief Habilita a interrupção do botão de pressão longa.
 *
 * A rotina de interrupção (`longPressButtonISR`) será chamada
 * a cada mudança de estado (**CHANGE**) do pino configurado.
 *
 * @param pin Pino do botão de pressão longa.
 */
void enableButtonInterruptChange(uint8_t pin);

/**
 * @brief Desabilita a interrupção do botão de pressão longa.
 *
 * Após esta chamada, o botão de long press deixará de gerar eventos.
 *
 * @param pin Pino do botão de pressão longa.
 */
void disableButtonInterruptChange(uint8_t pin);

// -----------------------------------------------------------------------------
// Funções de estado lógico
// -----------------------------------------------------------------------------

/**
 * @brief Verifica se o sistema está em estado de mute.
 *
 * Essa função verifica a flag de clique e alterna (`toggle`) o estado lógico.
 * Retorna o estado atual do sistema:
 * - `true`  → sistema mutado
 * - `false` → sistema ativo
 *
 * @return `true` se o botão foi pressionado e o sistema está mutado.
 * @return `false` caso contrário.
 */
bool wasMuted();

/**
 * @brief Verifica se o botão de pressão longa foi acionado.
 *
 * Retorna o valor da flag `flagLongPress`, que indica se o botão
 * foi mantido pressionado por mais de 15 segundos.
 *
 * @return `true` se o botão foi pressionado por mais de 15 segundos.
 * @return `false` caso contrário.
 */
bool wasButtonLongPressed();

/**
 * @brief Reseta o estado lógico do botão de mute.
 *
 * Define o estado interno (`isMuted`) como `false`,
 * independente de eventos anteriores. 
 * Útil durante inicializações ou reinicializações do sistema.
 */
void resetButtonState();


void resetShortPress();

bool wasButtonShortPress();


#endif
