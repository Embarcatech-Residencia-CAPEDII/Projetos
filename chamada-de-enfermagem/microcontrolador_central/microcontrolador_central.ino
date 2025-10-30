#include "wifi_utils.h"
#include "mqtt.h"

#include "display_LCD-2004_I2C.h"
#include "listNursingCall_utils.h"
#include "buttons.h"
#include "config.h"


// flag que indica se o botão de deletar foi pressionado uma vez e está aguardando confirmação
bool deletionConfirmation = false;


// ===== Funções de navegação =====
void handleNext() {  // ===== Botão Next (>)
  // Se estava em modo confirmação de deleção, atualiza o display, mostrando que a ação foi cancelada
  if (deletionConfirmation) {
    fixed_data();                  // Atualiza o display com os dados fixos
    deletionConfirmation = false;  // reset caso usuário navegue (cancela a exclusão)
  } else {
    // Avança para o próximo item da lista
    listCalls.next();
  }
  // Mostra o item atual
  showInfirmaryNumber(
    listCalls.getInfirmaryCurrent(),
    listCalls.hasNursingCall(),
    listCalls.getTotal());
}

void handlePrev() {  // ===== Botão Prev (<)
  // Se estava em modo confirmação de deleção, atualiza o display, mostrando que a ação foi cancelada
  if (deletionConfirmation) {
    fixed_data();                  // Atualiza o display com os dados fixos
    deletionConfirmation = false;  // reset caso usuário navegue (cancela a exclusão)
  } else {
    // Avança para o item anteriot da lista
    listCalls.prev();
  }
  // Mostra o item atual
  showInfirmaryNumber(
    listCalls.getInfirmaryCurrent(),
    listCalls.hasNursingCall(),
    listCalls.getTotal());
}


void handleDelete() {  // ===== Botão Delete
  // Primeiro clique: apenas exibe a mensagem de confirmação
  if (!deletionConfirmation) {
    showExclusionConfirm(listCalls.getInfirmaryCurrent());
    deletionConfirmation = true;
  } else {                      // Segundo clique: executa deleção
    listCalls.removeCurrent();  // Apaga o item selecionado
    fixed_data();               // Atualiza o display com os dados fixos
    showInfirmaryNumber(
      listCalls.getInfirmaryCurrent(),
      listCalls.hasNursingCall(),
      listCalls.getTotal());  // Mostra os dados no display
      // reseta o flag
      deletionConfirmation = false;
  }
}


void setup() {
  // Serial.begin(115200);
  logInit(LOG_MODE);

  if (!connectToWiFi()) {
    // Serial.println("WiFi não conectado.");
    log(LOG_WARN, "Falha ao conectar com WiFI.");
  }
  setupMQTT();

  // inicializa o display
  lcd2004_init();
  // Inicializa botões
  initButtons();

  showInfirmaryNumber(
      listCalls.getInfirmaryCurrent(),
      listCalls.hasNursingCall(),
      listCalls.getTotal());  // Mostra os dados no display
}

void loop() {

  checkAndReconnectWifi();
  checkMQTTConnected();

  // A função delay atrapalha o funcionamento dos botões
  // delay(1000);

  // client.disconnect();
  // if (!client.connected()){
  //   Serial.println("servidor mqtt desconectado");
  // }


  // Atualiza se tiver novos dados, mas se nenhum botão estiver pressionado
  if (listUpdated
      && button_next.state == HIGH
      && button_prev.state == HIGH
      && button_delete.state == HIGH
      && !deletionConfirmation) {

    showInfirmaryNumber(
      listCalls.getInfirmaryCurrent(),
      listCalls.hasNursingCall(),
      listCalls.getTotal());
    listUpdated = false;  // reseta a flag
  }

  // Habilita os botões somente se houver dados na lista
  if (listCalls.hasNursingCall()) {
    if (checkButton(button_next)) handleNext();
    if (checkButton(button_prev)) handlePrev();
    if (checkButton(button_delete)) handleDelete();
  }
}