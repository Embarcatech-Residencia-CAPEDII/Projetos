// Código para leitura de temperatura e umidade usando o sensor AHT10 com ESP32
// Os dados são exibidos no monitor serial a cada 3 segundos

// ===== INCLUDES E DEFINIÇÕES =====
#include <Wire.h>
#include "Adafruit_AHTX0.h"

#define AHT10_SDA 4
#define AHT10_SDL 5

// ===== OBJETOS GLOBAIS =====
Adafruit_AHTX0 aht;


// ===== STRUCTS =====
struct EnvironmentData {  //Estrutura para desacoplamento do sensor
  float temperature;
  float humidity;
  bool valid;
};


// ===== FUNÇÕES UTILITÁRIAS =====
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

void printSensorData(const EnvironmentData& data) {

  Serial.print("Temperatura: ");
  Serial.print(data.temperature);
  Serial.println(" °C");

  Serial.print("Umidade: ");
  Serial.print(data.humidity);
  Serial.println(" %");
}


// ===== FUNÇÕES DE INICIALIZAÇÃO =====
void initializeSensor() {

  Wire.begin(AHT10_SDA, AHT10_SDL);
  Serial.println("Iniciando sensor AHT10 ...");

  if (!aht.begin()) {
    Serial.println("Sensor AHT10 não inicializado");
    while (1) delay(10);
  }

  Serial.println("Sensor AHT10 inicializado com sucesso!");
}


// ===== FUNÇÃO SETUP =====
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  initializeSensor();
}


// ===== FUNÇÃO LOOP PRINCIPAL =====
void loop() {

  EnvironmentData sensorReading = readSensorData();
  if (sensorReading.valid) {
    printSensorData(sensorReading);
  }
  delay(3000);
}
