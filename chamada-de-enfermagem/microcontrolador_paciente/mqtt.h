#ifndef MQTT_H
#define MQTT_H
#include <PubSubClient.h>

extern PubSubClient client;



void connectMQTT();
void publishData();
void callback(char *topic, byte * payload, unsigned int length);

#endif