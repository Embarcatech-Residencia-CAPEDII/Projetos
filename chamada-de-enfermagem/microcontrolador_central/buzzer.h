#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

/**
 * @file buzzer.h
 * @brief Interface do módulo de controle do buzzer.
 * 
 * Este módulo define a interface para controle do buzzer,
 * incluindo inicialização, alternância de estado e desativação
 * do alerta sonoro. O buzzer é utilizado para indicar condições
 * anormais detectadas pelo sistema de monitoramento.
 */


/**
 * @brief Intervalo de tempo (em ms) entre as alternâncias do buzzer.
 * 
 * Controla a frequência com que o buzzer liga/desliga quando está em alerta.
 * No valor atual (2000 ms), o buzzer alterna a cada 2 segundos.
 */
constexpr unsigned long BUZZER_INTERVAL_MS = 1000;

/**
 * @brief Inicializa o buzzer configurando o pino como saída.
 * 
 * Deve ser chamada no setup() do programa antes de qualquer uso do buzzer.
 */
void buzzerInit();

/**
 * @brief Alterna o estado do buzzer (liga/desliga) com base no tempo.
 * 
 * @param now Tempo atual em milissegundos (obtido com millis()).
 * 
 * Alterna o buzzer somente se o intervalo definido em BUZZER_INTERVAL_MS
 * tiver sido atingido desde a última alternância.
 */
void toggleBuzzer();

/**
 * @brief Desliga o buzzer forçando nível lógico baixo no pino.
 * 
 * Deve ser chamada quando as condições de alerta não estão mais presentes.
 */
void enableSoundAlert();


bool doesHaveNotificationBuzzer();

#endif
