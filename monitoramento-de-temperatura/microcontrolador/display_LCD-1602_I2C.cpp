#include <Wire.h>                  // Inclui a biblioteca padrão para comunicação I2C
#include <LiquidCrystal_I2C.h>     // Biblioteca que controla o display LCD 1602 via módulo I2C
#include <math.h>                  // Biblioteca de funções matemáticas. Usada para "fabs(...)" valor absoluto de float/double.
#include "display_LCD-1602_I2C.h"  // Inclusão do arquivo de cabeçalho do módulo que contém as declarações de funções
#include "log.h"


const uint8_t I2C_ADDR = 0x27;  // Endereço padrão do módulo I2C do LCD
// Define quantidade de linhas e colunas do LCD (16 colunas e 2 linhas)
const uint8_t LCD_COLUMNS = 16;
const uint8_t LCD_LINES = 2;

// Mensagem padrão exibida no display quando a temperatura ultrapassa o limite
#define TEMPERATURE_ALERT_MESSAGE F("ALERT_T: ")
// Mensagem padrão usada para exibir a temperatura quando está dentro do limite
#define TEMPERATURE_MESSAGE F("T: ")
// Mensagem padrão exibida no display quando a umidade ultrapassa o limite
#define HUMIDITY_ALERT_MESSAGE F("ALERT_U: ")
// Mensagem padrão usada para exibir a umidade quando está dentro do limite
#define HUMIDITY_MESSAGE F("U: ")

// Variáveis auxiliares para guardar último valor de temperatura e umidade
float lastTemperature = -1000.0;
float lastHumidity = -1000.0;

// Limiares mínimos de atualização para evitar reescrever o display sem necessidade
const float TEMP_UPDATE_THRESHOLD = 0.01;
const float HUMI_UPDATE_THRESHOLD = 0.01;

bool isBacklightTurnOn = true;
unsigned long lastTimeBackligthTurnOn = 0;

// Instancia do LCD
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);


// Função que inicializa o Display
void lcd1602_init() {
  Wire.begin();         // para garantir que I2C foi inicializada
  lcd.init();           // Inicializa o display e a comunicação I2C
  lcd.backlight();      // Liga a luz de fundo do display
  lcd.setCursor(0, 0);  // Posiciona o cursor na coluna 0, linha 0
  // Usa F(" ") para economizar RAM ao imprimir
  lcd.print(F("Iniciando..."));  // Escreve a mensagem inicial no display
  delay(300);                    // Aguarda 300 ms
  lcd.clear();                   // Limpa completamente o display
}


/*Função de Limpeza de Linha
    Parâmetro:
      - line: linha do display a ser limpa (uint8_t) */
void clearLine(uint8_t line) {
  if (line >= LCD_LINES) return;  // valida linha
  // Posiciona o cursor no início da linha indicada
  lcd.setCursor(0, line);
  // Imprime 16 espaços (uma linha cheia) para apagar o conteúdo anterior
  for (int i = 0; i < LCD_COLUMNS; i++) {
    lcd.print(F(" "));
  }
}


/* Função genérica para exibir o valor de temperatura ou umidade
   Parâmetros:
     - line: linha escolhida do display (0 = primeira, 1 = segunda)
     - normalMsg/alertMsg: ponteiros do tipo __FlashStringHelper (strings guardadas na Flash via F()).
     - value: valor de temperatura ou umidade a mostrar.
     - alert: se true, usa a mensagem de alerta; senão, a normal
     - unit: sufixo de unidade (ex.: "%" ou "\xDF""C").*/
void show_value(uint8_t line,
                const __FlashStringHelper* normalMsg,
                const __FlashStringHelper* alertMsg,
                float value,
                bool alert,
                const char* unit) {
  // Limpa a linha antes de escrever
  clearLine(line);
  // Posição inicial da linha
  lcd.setCursor(0, line);
  // escolhe e imprime a mensagem apropriada diretamente da Flash
  //    Se alert for verdadeiro, exibe a mensagem de alerta. Caso contrário, mostra só a mensagem normal "U: " ou "T: "
  lcd.print(alert ? alertMsg : normalMsg);
  // Imprime o valor no display com 2 casa decimal
  lcd.print(value, 2);
  // Imprime a Unidade ("°C" ou "%")
  lcd.print(unit);
}


/* Função que exibe os valores de temperatura e umidade no display
   Parâmetros:
     - temp: valor da temperatura (float)
     - humi: valor da umidade (float)
     - alertTemp: indica se a temperatura ultrapassou o limite (true = alerta)
     - alertHumi: indica se a umidade ultrapassou o limite (true = alerta)*/
void lcd1602_showData(float temp, float humi, bool alertTemp, bool alertHumi) {

  // Só chama a função se a temperatura mudou além do limiar (>= 0,01)
  if (fabs(temp - lastTemperature) >= TEMP_UPDATE_THRESHOLD) {
    // Linha 0: temperatura com prefixo "T: " ou "ALERT_T: ", sufixo "°C".
    show_value(0,
               TEMPERATURE_MESSAGE,
               TEMPERATURE_ALERT_MESSAGE,
               temp,
               alertTemp,
               "\xDF"
               "C");
    lastTemperature = temp;  // atualiza último valor
  }

  // Só chama a função se a umidade mudou além do limiar
  if (fabs(humi - lastHumidity) >= HUMI_UPDATE_THRESHOLD) {
    // Linha 1: umidade com prefixo "U: " ou "ALERT_U: ", sufixo "%".
    show_value(1,
               HUMIDITY_MESSAGE,
               HUMIDITY_ALERT_MESSAGE,
               humi,
               alertHumi,
               "%");
    lastHumidity = humi;  // atualiza último valor
  }
}

void turnOnBacklight() {
  if (isBacklightTurnOn == false) {
    lcd.backlight();
    isBacklightTurnOn = true;
  }
}

void turnOffBacklight() {
  if (isBacklightTurnOn == true) {
    lcd.noBacklight();
    isBacklightTurnOn = false;
  }
}

void handleBacklightLCD(bool isAlertState, bool commandTurnOnLCD) {

  if (isAlertState) {
    turnOnBacklight();
  } else {

    unsigned long now = millis();
    if (commandTurnOnLCD) {
      turnOnBacklight();
      lastTimeBackligthTurnOn = now;

    } else {
      // log(LOG_INFO,"começou a contagem");
      if (now - lastTimeBackligthTurnOn > 10 * 1000) {
        turnOffBacklight();
      }
    }

  }
}
