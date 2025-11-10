#ifndef DISPLAY_LCD_1602_I2C_H // Verifica se o cabeçalho já foi incluído
#define DISPLAY_LCD_1602_I2C_H // Define o identificador do cabeçalho para evitar inclusões múltiplas

// Declara a função que inicializa o display LCD 1602 via interface I2C
// Deve ser chamada no setup() para configurar o display
extern void lcd1602_init();

/* Declara a função que exibe os valores de temperatura e umidade no display
   Parâmetros:
     - temp: valor da temperatura (float)
     - humi: valor da umidade (float)
     - alertTemp: indica se a temperatura ultrapassou o limite (true = alerta)
     - alertHumi: indica se a umidade ultrapassou o limite (true = alerta)*/
extern void lcd1602_showData(float temp, float humi, bool alertTemp, bool alertHumi);

extern void handleBacklightLCD(bool isAlertState, bool commandTurnOnLCD);

#endif // Final da diretiva de proteção contra múltiplas inclusões