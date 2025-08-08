#ifndef SENSOR_H
#define SENSOR_H

#include <Adafruit_AHTX0.h>

struct EnvironmentData {
  float temperature;
  float humidity;
  bool valid;
};

void initializeSensor();
EnvironmentData readSensorData();
void printSensorData(const EnvironmentData& data);

extern Adafruit_AHTX0 aht;

#endif
