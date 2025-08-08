#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

#include <PubSubClient.h>

extern PubSubClient client;

void setupMQTT();
void checkMQTTConnected();
void publishFloat(const char* topic, float value);
void publishSensorData(float temperature, float humidity);

#endif
