#include "peripherals.h"
#include <Arduino.h>

const int BUTTON = 5;

void setupPeripherals(){
  pinMode(BUTTON, INPUT_PULLUP);
}

bool readButton() {
  return digitalRead(BUTTON); // botão pressionado
}