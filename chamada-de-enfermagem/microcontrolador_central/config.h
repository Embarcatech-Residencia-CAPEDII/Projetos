#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H

#include "log.h"

constexpr LogLevel LOG_MODE = LOG_INFO;     // Configuração de modo para monitoriamento serial | opções com grau crescente de verbosidade: LOG_NONE, LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG


const constexpr uint8_t PIN_BUZZER = 15;        //pino para atuação do buzzer
const constexpr uint8_t PIN_LED = 2;           //pino para utilizaçao da led

const uint8_t NUMBER_SOUND_ALERTS = 5;

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