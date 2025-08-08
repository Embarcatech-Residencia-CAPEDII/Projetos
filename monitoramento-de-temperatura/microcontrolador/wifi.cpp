#include <ESP8266WiFi.h>
#include "wifi.h"

const char* WIFI_SSID = "xxxxxxxxxx";
const char* WIFI_PASSWORD = "xxxxxxxx";

/**
 * Conecta com WiFi e retorna valor true para conexão bem sucedida.
 */
bool connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a ");
  Serial.println(WIFI_SSID);

  unsigned long timeout = millis() + 10000;
  while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Conectado ao Wi-Fi");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("Falha na conexão Wi-Fi");
  return false;
}
