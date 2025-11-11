#include "wifi_utils.h"
#include "mqtt.h"

#include "display_LCD-2004_I2C.h"
#include "listNursingCall_utils.h"
#include "buttons.h"
#include "config.h"
#include "buzzer.h"
#include "led.h"

// flag que indica se o botão de deletar foi pressionado uma vez e está aguardando confirmação
bool deletionConfirmation = false;


// ===== Funções de navegação =====
void handleNext() {  // ===== Botão Next (>)
  // Se estava em modo confirmação de deleção, atualiza o display, mostrando que a ação foi cancelada
  if (deletionConfirmation) {
    fixed_data();                  // Atualiza o display com os dados fixos
    deletionConfirmation = false;  // reset caso usuário navegue (cancela a exclusão)

    // Quando clica 'next' e esta no esta no modo confirmação, garante que NÃO está bloqueado remover current (caso tenha atigido o limite de inserção na lista)
    listCalls.setDoNotRemoveCurrent(false); // vira false -> pode remover current
  } 
  else {
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

    // Quando clica 'prev' e esta no esta no modo confirmação, garante que NÃO está bloqueado remover current (caso tenha atigido o limite de inserção na lista)
    listCalls.setDoNotRemoveCurrent(false); // vira false - pode remover current
  } 
  else {
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

    // Quando clica 1 vez no 'del' não pode remover current, caso tenha atigido o limite de inserção na lista
    listCalls.setDoNotRemoveCurrent(true); // Aqui diz é true, não pode remover current
  } 
  else {  // Segundo clique: executa deleção

    /* ___Pública (marcar com concluído o chamado) via MQTT*/
    int infirmary = listCalls.getInfirmaryCurrent();
    const char* idDevice = listCalls.getIdCurrent();
    // Chama a função de públicar o ID do dispositivo e o número da enfermaria (tranforma em float o infirmary)
    publicReponseDivice(idDevice, (float)infirmary);


    if (listCalls.removeCurrent()) {  // Apaga o item selecionado
      log(LOG_INFO,"Chamada removida com sucesso!");
    } else {
      log(LOG_ERROR,"Erro ao remover a chamada na lista!");
    }
    fixed_data();               // Atualiza o display com os dados fixos
    showInfirmaryNumber(
      listCalls.getInfirmaryCurrent(),
      listCalls.hasNursingCall(),
      listCalls.getTotal());  // Mostra os dados no display
    // reseta o flag
    deletionConfirmation = false;

    // Ao marcar o chamado como resolvido, reseta a flag, indicando se atingir o limite pode remover o current
    listCalls.setDoNotRemoveCurrent(false); // vira false - pode remover current
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

  ledInit();

  // inicializa buzzer
  buzzerInit();

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
  // aciona led se houver algum chamado
  if (listCalls.hasNursingCall()) {
    if (checkButton(button_next)) handleNext();
    if (checkButton(button_prev)) handlePrev();
    if (checkButton(button_delete)) handleDelete();

    toggleLed();
  } else {
    turnOffLed();
  }

  if (doesHaveNotificationBuzzer()) {
    toggleBuzzer();
  }
}