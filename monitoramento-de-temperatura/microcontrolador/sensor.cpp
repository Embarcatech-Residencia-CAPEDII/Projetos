#include "sensor.h"
#include <Wire.h>

Adafruit_AHTX0 aht;

/**
 * Inicializa o sensor AHT10.
 */
void initializeSensor() {
  Wire.begin(4, 5);  // Pinos fixos aqui ou passados por parâmetro depois

  if (!aht.begin()) {
    Serial.println("Sensor AHT10 não inicializado");
    while (1) delay(10);
  }

  Serial.println("Sensor AHT10 inicializado com sucesso!");
}


/**
 * Lê os dados de temperatura e umidade do sensor AHT10.
 * 
 * @return Estrutura contendo temperatura (°C), umidade (%) e status de validade dos dados.
 */
EnvironmentData readSensorData() {
  sensors_event_t hum, temp;
  aht.getEvent(&hum, &temp);

  EnvironmentData data;
  if (isnan(temp.temperature) || isnan(hum.relative_humidity)) {
    data.valid = false;
    return data;
  }

  data.temperature = temp.temperature;
  data.humidity = hum.relative_humidity;
  data.valid = true;

  return data;
}

/**
 * Exibe os dados do sensor no monitor serial.
 *
 *@param data - Estrutura contendo os valores de temperatura (°C) e umidade (%).
 */
void printSensorData(const EnvironmentData& data) {
  Serial.print("Temperatura: ");
  Serial.print(data.temperature);
  Serial.println(" °C");

  Serial.print("Umidade: ");
  Serial.print(data.humidity);
  Serial.println(" %");
}
