#include "wifi_utils.h"
#include "mqtt.h"

void setup() {
  Serial.begin(115200);

  if (!connectToWiFi()) {
    Serial.println("WiFi não conectado.");
  }
  setupMQTT();
}

void loop() {
  
  checkAndReconnectWifi();
  checkMQTTConnected();

  delay(1000);
  client.disconnect(); 
  if (!client.connected()){
    Serial.println("servidor mqtt desconectado");
  }
}
