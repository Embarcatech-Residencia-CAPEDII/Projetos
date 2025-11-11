#include "buzzer.h"
#include "log.h"
#include "config.h"

bool isBuzzerNotificationEnabled = false;

uint8_t numberAlertIssued = 0;

// ------------------------------------------------------------
// Variável de controle interno do módulo
// ------------------------------------------------------------

/**
 * @brief Guarda o tempo da última alternância do buzzer.
 * 
 * Usada para calcular o intervalo entre mudanças de estado,
 * evitando que o buzzer alterne em frequência muito alta.
 */
unsigned long lastBuzzerPlayTime = 0;

// ------------------------------------------------------------
// Implementação das funções públicas declaradas em buzzer.h
// ------------------------------------------------------------

void buzzerInit() {
  // Configura o pino do buzzer como saída
  // e garante que inicie desligado
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
}



void toggleBuzzer() {

  unsigned long now = millis();


  if (now - lastBuzzerPlayTime >= BUZZER_INTERVAL_MS) {
    lastBuzzerPlayTime = now;

    if (numberAlertIssued >= NUMBER_SOUND_ALERTS) {
      isBuzzerNotificationEnabled = false;
      digitalWrite(PIN_BUZZER, LOW);
      return;
    }

    // Inverte o estado atual do pino do buzzer:
    // se estava HIGH passa para LOW, e vice-versa
    bool stateButton = !digitalRead(PIN_BUZZER);


    digitalWrite(PIN_BUZZER, stateButton);

    if (stateButton == HIGH) {
      numberAlertIssued++;
    }

    log(LOG_DEBUG, "Buzzer alternou o estado");
  }
}

bool doesHaveNotificationBuzzer() {
  return isBuzzerNotificationEnabled;
}

void enableSoundAlert() {
  isBuzzerNotificationEnabled = true;
  numberAlertIssued = 0;
}
