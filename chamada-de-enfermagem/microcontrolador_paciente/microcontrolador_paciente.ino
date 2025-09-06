#include "wifi.h"
#include "mqtt.h"
#include "peripherals.h"

unsigned long lastPublish = 0;
const long interval = 60000;

void setup() {

  Serial.begin(115200);
  setupPeripherals();
  connectWiFi();
  connectMQTT();

  lastPublish = millis() - interval;
}

void loop() {

  if (!client.connected()) {
    connectMQTT();
  }

  client.loop();
  checkConnection();

  //se o botao for pressionado, aguarda 1 minuto para libera-lo(para testes, posteriormente sera condicionado a resposta da enfermeira)
  if ((millis() - lastPublish > interval) && !readButton()) {
      Serial.println("Botão pressionado!");
      publishData();
      lastPublish = millis();
    }
  }