#include "wifi_utils.h"
#include "mqtt.h"
#include "server.h"

#include "display_LCD-2004_I2C.h"
#include "listNursingCall_utils.h"
#include "jsonDataProcessing.h"
#include "buttons.h"
#include "config.h"
#include "buzzer.h"
#include "led.h"
#include "config_storage.h"


// flag que indica se o botão de deletar foi pressionado uma vez e está aguardando confirmação
bool deletionConfirmation = false;

// flag que indica se a mensagem de falha ddo wifi já foi exibida (detectar transição de falha para normal).
// bool wifiFailureDisplayed = false;  // Usada para evitar desenhar a tela de erro repetidamente a cada iteração do loop

// flag que indica se a mensagem de falha ddo wifi já foi exibida (detectar transição de falha para normal).
// bool wifiFailureDisplayed = false;  // Usada para evitar desenhar a tela de erro repetidamente a cada iteração do loop

ESP8266WebServer server(80);

// ===== Funções de navegação =====
void handleNext() {  // ===== Botão Next (>)
  // Se estava em modo confirmação de deleção, atualiza o display, mostrando que a ação foi cancelada
  if (deletionConfirmation) {
    deletionConfirmation = false;  // reset caso usuário navegue (cancela a exclusão)

    // Quando clica 'next' e esta no esta no modo confirmação, garante que NÃO está bloqueado remover current (caso tenha atigido o limite de inserção na lista)
    listCalls.setDoNotRemoveCurrent(false);  // vira false -> pode remover current
  } else {
    if (listCalls.hasNursingCall()) {
      // Avança para o próximo item da lista
      listCalls.next();
    }
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
    deletionConfirmation = false;  // reset caso usuário navegue (cancela a exclusão)

    // Quando clica 'prev' e esta no esta no modo confirmação, garante que NÃO está bloqueado remover current (caso tenha atigido o limite de inserção na lista)
    listCalls.setDoNotRemoveCurrent(false);  // vira false - pode remover current
  } else {
    if (listCalls.hasNursingCall()) {
      // Avança para o item anteriot da lista
      listCalls.prev();
    }
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

    // Se a tela anterior não for a MAIN, não pode mostrar a tela de confirmação de exclusão
    if (currentScreen != SCREEN_MAIN) {
      return;
    }

    showExclusionConfirm(listCalls.getInfirmaryCurrent());
    deletionConfirmation = true;

    // Quando clica 1 vez no 'del' não pode remover current, caso tenha atigido o limite de inserção na lista
    listCalls.setDoNotRemoveCurrent(true);  // Aqui diz é true, não pode remover current
  } else {                                  // Segundo clique: executa deleção

    // Se a tela anterior não for a de confirmação de exclusão, não pode continuar
    if (currentScreen != SCREEN_EXCLUSION_CONFIRM) {
      return;
    }

    /* ___Pública (marcar com concluído o chamado) via MQTT*/
    const char* infirmary = listCalls.getInfirmaryCurrent();
    const char* idDevice = listCalls.getIdCurrent();
    // Chama a função de públicar o ID do dispositivo e o número da enfermaria (tranforma em float o infirmary)

    char buffer[256];

    // flag qu indica se deu certo publicar ou não
    bool wasPublished = publicReponseDivice(idDevice, MQTT_PUBLICATION_TOPIC, createJsonPayload(buffer, sizeof(buffer), infirmary));


    // Caso tenha falhado publicar o chamado, mostra a tela indicando a falha
    if (!wasPublished) {
      showFailureMessage(MESSAGE_MQTT);
    }
  }
}


void setup() {
  // Serial.begin(115200);
  logInit(LOG_MODE);
  // inicializa o display
  lcd2004_init();


  // Inicializa botões
  initButtons();

  enableButtonInterruptChange();

  ledInit();

  // inicializa buzzer
  buzzerInit();

  initConfigStorage();
  cfg = loadConfig();

  if (!connectToWiFi()) {
    // Serial.println("WiFi não conectado.");
    log(LOG_WARN, "Falha ao conectar com WiFI.");
  }

  setupMQTT();

  showInfirmaryNumber(
    listCalls.getInfirmaryCurrent(),
    listCalls.hasNursingCall(),
    listCalls.getTotal());  // Mostra os dados no display
}

void loop() {


  if (isConfigurationMode() || !cfg.valid) {

    turnOnLed();
    createAccessPoint();
    startServer(&server);
    server_handle_loop(&server);

    // log(LOG_INFO, "modo configuraçao");

  } else {

    if (checkAndReconnectWifi()) {
      checkMQTTConnected();
    } else {                             // não há conexão
      showFailureMessage(MESSAGE_WIFI);  // Mensagem que indica que não há conexão Wi-Fi**
    }


    stopServer(&server);

  if (hasOKMessage) {
    // log(LOG_INFO,listCalls.getIdCurrent());
    if (listCalls.removalById(calledToBeErased)) {  // Apaga o item selecionado
      log(LOG_INFO, "Chamada removida com sucesso!");
    } else {
      log(LOG_ERROR, "Erro ao remover a chamada na lista!");
    }
    showInfirmaryNumber(
      listCalls.getInfirmaryCurrent(),
      listCalls.hasNursingCall(),
      listCalls.getTotal());  // Mostra os dados no display

      // Ao marcar o chamado como resolvido, reseta a flag, indicando se atingir o limite pode remover o current
      deletionConfirmation = false;
      listCalls.setDoNotRemoveCurrent(false);  // vira false - pode remover current
      hasOKMessage = false;
    }


    // Atualiza se tiver novos dados, mas se nenhum botão estiver pressionado e se tiver na tela de navegação
    if (listUpdated
        && button_next.state == HIGH
        && button_prev.state == HIGH
        && button_delete.state == HIGH
        && currentScreen == SCREEN_MAIN
        && !deletionConfirmation) {

      showInfirmaryNumber(
        listCalls.getInfirmaryCurrent(),
        listCalls.hasNursingCall(),
        listCalls.getTotal());

      listUpdated = false;  // reseta a flag
    }

    // Verificações para poder sair das telas IP/Erro wifi/MQTT
    if (checkButton(button_next)) handleNext();
    if (checkButton(button_prev)) handlePrev();

    // Habilita o botão de delete somente se houver dados na lista
    // aciona led se houver algum chamado
    if (listCalls.hasNursingCall()) {
      // if (checkButton(button_next)) handleNext();
      // if (checkButton(button_prev)) handlePrev();
      if (checkButton(button_delete)) handleDelete();

      toggleLed();
    } else {
      turnOffLed();
    }

    if (doesHaveNotificationBuzzer()) {
      toggleBuzzer();
    }
  }
}