#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H

#include "log.h"

constexpr LogLevel LOG_MODE = LOG_INFO;     // Configuração de modo para monitoriamento serial | opções com grau crescente de verbosidade: LOG_NONE, LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG


constexpr char* WIFI_SSID = "xxxxxxxxxxxxxx";
constexpr char* WIFI_PASSWORD = "xxxxxxxxxxxxxx";


constexpr char* MQTT_SERVER = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
constexpr char* MQTT_USER = "xxxxxxxxxxxxxx";
constexpr char* MQTT_PASS = "xxxxxxxxxxxxxx";
constexpr char* MQTT_DEVICE_ID = "xxxxxxxxxxxxxx";
constexpr  char* MQTT_PUBLICATION_TOPIC = "dispositivos/enfermagem";
constexpr  char* MQTT_SUBSCRIPTION_TOPIC = "dispositivos/posto_enfermagem";
constexpr  int mqtt_port = 8883;

#endif