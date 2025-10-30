#include <ESP8266WiFi.h>  // Biblioteca oficial ESP32 para conexão Wi-Fi
#include "wifi_utils.h"   // Declarações das funções públicas do módulo Wi-Fi
#include "config.h"       // Constantes globais do projeto (SSID e senha)
#include "log.h"

// ------------------------------------------------------------
// Variáveis internas do módulo
// ------------------------------------------------------------

// ------------------------------------------------------------
// Funções uteis para o módulo
// ------------------------------------------------------------
const char* IPparserToConstChar(IPAddress ip) {
  static char buffer[16];
  snprintf(buffer, sizeof(buffer), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);

  return buffer;
}



/**
 * @brief Armazena o timestamp (millis) da última tentativa de reconexão.
 * Usado para limitar a frequência de tentativas automáticas.
 */
static unsigned long lastConnectionAttemp = 0;

/**
 * @brief Intervalo mínimo entre tentativas de reconexão (ms)
 * Evita reconectar muito rápido quando o Wi-Fi está instável.
 */
static const unsigned long reconnectInterval = 2000;  // 10 segundos

// ------------------------------------------------------------
// Implementação das funções públicas do módulo
// ------------------------------------------------------------

/**
 * @brief Conecta o ESP32 à rede Wi-Fi definida em config.h.
 * 
 * Aguarda até 10 segundos pela conexão, imprimindo progresso no Serial.
 * 
 * @return true se a conexão foi bem sucedida, false caso contrário.
 */
bool connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // inicia conexão
  log(LOG_INFO, "Conctandoo ao wifi");

  // Define um timeout de 10 segundos
  unsigned long timeout = millis() + 10000;

  // Aguarda até a conexão ou até o timeout
  while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);  // aguarda meio segundo entre tentativas
    // Serial.print(".");     // imprime ponto para indicar progresso
  }

  // Serial.println();

  // Verifica se a conexão foi bem sucedida
  if (WiFi.status() == WL_CONNECTED) {
    log(LOG_INFO, "Conectado ao Wi-Fi: %s", WIFI_SSID);
    log(LOG_INFO, "IP: %s", IPparserToConstChar(WiFi.localIP()));  // mostra o IP atribuído
    return true;
  }

  log(LOG_ERROR, "Falha na conexão Wi-Fi");
  return false;
}

/**
 * @brief Verifica a conexão Wi-Fi e tenta reconectar automaticamente.
 * 
 * Essa função deve ser chamada periodicamente no loop() do programa.
 * Se o Wi-Fi estiver desconectado, ela espera o intervalo definido
 * em reconnectInterval antes de tentar reconectar.
 */
void checkAndReconnectWifi() {
  // Se já está conectado, não faz nada
  if (WiFi.status() == WL_CONNECTED) return;

  unsigned long now = millis();

  // Verifica se já passou o intervalo de reconexão
  if (now - lastConnectionAttemp >= reconnectInterval) {
    lastConnectionAttemp = now;

    log(LOG_INFO, "WiFi desconectado");
    WiFi.disconnect();  // garante que está desconectado
    log(LOG_INFO, "Conectando ao wifi ");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // inicia reconexão

    // Se reconectar com sucesso, exibe informações
    if (WiFi.status() == WL_CONNECTED) {
      log(LOG_INFO, "Conectado ao Wi-Fi");
      log(LOG_INFO, "IP: %s", IPparserToConstChar(WiFi.localIP()));  // mostra o IP atribuído
    }
  }
}

/**
 * @brief Retorna se o ESP32 está conectado ao Wi-Fi.
 * 
 * @return true se conectado, false caso contrário.
 */
bool wifiIsConnected() {
  return (WiFi.status() == WL_CONNECTED);
}
