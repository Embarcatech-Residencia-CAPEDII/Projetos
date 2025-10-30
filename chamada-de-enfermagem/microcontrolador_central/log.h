#ifndef LOG_H
#define LOG_H

#include <Arduino.h>
#include <stdarg.h> 

/**
 * @file log.h
 * @brief Declarações para o sistema de logging do projeto.
 *
 * Este módulo define níveis de log e funções utilitárias
 * para registrar mensagens em diferentes níveis de severidade.
 * Pode ser configurado para controlar a quantidade de informações
 * enviadas à saída serial durante a execução.
 */

/**
 * @enum LogLevel
 * @brief Define os níveis de log disponíveis.
 */
enum LogLevel {
  LOG_NONE = 0,   /**< Nenhum log será exibido */
  LOG_ERROR = 1,  /**< Mensagens de erro (alta prioridade) */
  LOG_WARN = 2,   /**< Mensagens de aviso */
  LOG_INFO = 3,   /**< Mensagens informativas */
  LOG_DEBUG = 4   /**< Mensagens de depuração (mais verboso) */
};

/**
 * @brief Variável global que armazena o nível de log atual.
 *
 * Controla quais mensagens serão exibidas.
 * Deve ser inicializada pela função logInit().
 */
extern LogLevel CURRENT_LOG_LEVEL;

/**
 * @brief Inicializa o sistema de log.
 *
 * Configura o nível de log inicial e inicia a comunicação serial.
 *
 * @param level Nível de log desejado (LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG).
 */
void logInit(LogLevel level);

/**
 * @brief Registra uma mensagem no log.
 *
 * Exibe a mensagem apenas se o nível informado for maior ou igual
 * ao nível global configurado em CURRENT_LOG_LEVEL.
 *
 * @param level Nível da mensagem (LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG).
 * @param fmt   String formatada no estilo printf.
 * @param ...   Argumentos variáveis para formatação.
 */
void log(LogLevel level, const char* fmt, ...);

#endif
