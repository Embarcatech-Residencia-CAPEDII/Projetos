#include "log.h"
#include <Arduino.h>
#include <stdarg.h>

/**
 * @file log.cpp
 * @brief Implementação de funções de logging para depuração e monitoramento do sistema.
 *
 * Este módulo fornece um mecanismo centralizado para registrar mensagens no Serial
 * em diferentes níveis de severidade (ERRO, WARN, INFO, DEBUG).
 */

// Variável global que define o nível atual de log.
// Por padrão, está configurado como DEBUG (mostra todos os logs).
LogLevel CURRENT_LOG_LEVEL = LOG_DEBUG;

/**
 * @brief Inicializa o sistema de logs.
 *
 * - Define o nível de log inicial.
 * - Inicia a comunicação Serial a 115200 baud.
 * - Registra uma mensagem indicando que o log foi iniciado.
 *
 * @param level Nível de log a ser utilizado (LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG).
 */
void logInit(LogLevel level) {
  CURRENT_LOG_LEVEL = level;
  Serial.begin(115200);
  log(LOG_INFO, "Log iniciado");
}

/**
 * @brief Registra uma mensagem de log com formato customizado.
 *
 * Esta função formata a mensagem recebida e imprime na Serial,
 * respeitando o nível atual de log definido.
 *
 * @param level Nível da mensagem (ERRO, WARN, INFO, DEBUG).
 * @param fmt   String de formato (similar ao printf).
 * @param ...   Argumentos variáveis correspondentes ao formato.
 */
void log(LogLevel level, const char* fmt, ...) {
  // Ignora a mensagem se o nível atual for menor ou logging estiver desabilitado
  if (CURRENT_LOG_LEVEL < level || CURRENT_LOG_LEVEL == LOG_NONE) return;

  // Tag textual do nível de log
  const char* tag;
  switch (level) {
    case LOG_ERROR: tag = "ERRO"; break;
    case LOG_WARN:  tag = "WARN"; break;
    case LOG_INFO:  tag = "INFO"; break;
    case LOG_DEBUG: tag = "DEBUG"; break;
    default:        tag = "LOG"; break;
  }

  // Buffer para a mensagem formatada
  char buf[140];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  // Impressão no Serial com prefixo do nível
  Serial.print("[");
  Serial.print(tag);
  Serial.print("] ");
  Serial.println(buf);
}
