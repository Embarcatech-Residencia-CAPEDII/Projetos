#include "jsonDataProcessing.h"
#include "listNursingCall_utils.h"
#include <ArduinoJson.h>
#include <cstring>
#include "log.h"


/*  Exemplo JSON (payload) recebido:
{
  "id": "Enfermagem1",
  "estado": "emergencia",
  "mensagem": "Ligar LED",
  "room_number": "9",
  "local": "Enfermaria",
  "comando": "ligar"
}*/


// Capacidade do StaticJsonDocument
static constexpr size_t JSON_CAPACITY = 256;


// Converte string para número (Retorna true se conversão e limites estiverem ok e grava resultado em room_number)
//    int& room_number: referência para variável que receberá o resultado
//    const char* room_str: string a ser convertida
bool roomNumberConversion(int *room_number, const char* room_str) {
  if (room_str == nullptr) return false;  // Verifica se string é nula - retorna falha

  char* endptr = nullptr;                  // ponteiro que indica onde a conversão parou
  long v = strtol(room_str, &endptr, 10);  // conversão string para long

  // Verifica se conversão foi completa
  if (endptr == room_str) {
    log(LOG_ERROR, "room_number string inválida");
    return false;
  }

  // Há caracteres extras após número
  if (*endptr != '\0') {
    log(LOG_ERROR, "room_number string inválida (caracteres extras)");
    return false;
  }

  *room_number = (int)v;              // Atribui valor convertido via referência
  return true;                        // Retorna sucesso - conversão válida
}


// Validação básica do payload JSON bruto
bool validateJsonFields(const char* payload, unsigned int length) {
  if (payload == nullptr) return false;
  if (length == 0) return false;

  if (length > 1024) {  // limite de segurança
    log(LOG_ERROR, "Payload muito grande!");
    return false;
  }

  // Checa formato mínimo - deve começar com '{'
  if (payload[0] != '{') {
    log(LOG_ERROR, "Payload não parece JSON");
    return false;
  }

  return true;  // Payload é válido
}


// processa os dados JSON recebido do MQTT
void processing_json_MQTT(byte* payload, unsigned int length) {
  // Cria um documento JSON estático na stack (memória temporária) com capacidade de 256 bytes para armazenar o JSON parseado.
  StaticJsonDocument<256> doc;  // Ou <512>

  // Tenta converter (parsear) os dados brutos em estrutura JSON
  DeserializationError error = deserializeJson(doc, payload, length);

  // Verifica se ocorreu algum erro no parse do JSON
  if (error) {
    log(LOG_ERROR, "Erro no parse: ");
    // converte o erro em string legível e a imprimi no monitor serial
    log(LOG_ERROR, error.c_str());
    // Sai da função prematuramente se houve erro no parse
    return;
  }

  // ____Extração dos Dados do JSON
  // Acessa o campo "id" do JSON e armazena como ponteiro para string constante
  const char* id = doc["id"];  // Ex: "Enfermagem1"
  // Extrai campo "estado" do JSON
  const char* estado = doc["estado"];  // Ex: "emergencia"
  // Extrai campo "mensagem" do JSON
  const char* mensagem = doc["mensagem"];  // Ex: "Ligar LED"


  // // Extrai campo "mensagem" do JSON
  // const char* room_number = doc["room_number"];
  int room_number;

  // Verifica se o campo "room_number" do objeto 'doc' é um número inteiro
  if (doc["room_number"].is<int>()) {
    // Se for inteiro, converte diretamente para int
    room_number = doc["room_number"].as<int>();
    

  } else if (doc["room_number"].is<const char*>()) {  // Caso contrário, verifica se o campo é string (const char*)

    // // Obtém a string do campo "room_number"
    const char* room_str = doc["room_number"].as<const char*>();
    // // Ponteiro usado por strtol() para identificar onde a conversão parou
    // char* endptr = nullptr;
    // // Converte a string para número inteiro longo (base 10)
    // long v = strtol(rn, &endptr, 10);

    // // Verifica se a conversão falhou:
    // // - Se endptr == rn, nenhum dígito foi convertido.
    // // - Se *endptr != '\0', há caracteres não numéricos após o número.
    // if (endptr == rn || *endptr != '\0') {
    //   log(LOG_ERROR, "room_number string inválida");
    //   return;  // não adiciona o dado (saída antecipada)
    // }

    // // Converte o valor long obtido para int
    // room_number = (int)v;
    if(!roomNumberConversion(&room_number, room_str)){
      return;
    }

  } else {  // Caso o tipo de 'room_number' não seja nem inteiro nem string
    log(LOG_ERROR, "room_number não é string nem número!");
    return;  // não adiciona o dado
  }
  // Verificação adicional: garante que o número esteja dentro do intervalo aceitável (Modificar de acordo com o números dos quartos)
  if (room_number <= 0 || room_number > 9999) {
    log(LOG_ERROR, "room_number fora dos limites aceitáveis");
    return;  // Valor inválido, interrompe a execução
  }

  // if (!roomNumberValid) {  // Verifica se conversão foi bem-sucedida
  //   return;                // Se falhou, sai da função
  // }

  // Extrai campo "mensagem" do JSON
  const char* local = doc["local"];  // Ex: "Enfermagem"
  // Extrai campo "comando" do JSON
  const char* comando = doc["comando"];  // Ex: "ligar"

  log(LOG_INFO,
      " -> ID: %s\n"
      " -> estado: %s\n"
      " -> mensagem: %s\n"
      " -> Num. enfermaria: %d\n"
      " -> local: %s\n"
      " -> comando: %s\n",
      id,
      estado,
      mensagem,
      room_number,
      local,
      comando);

  // Adiciona na lista
  listCalls.add(room_number,id);
  listUpdated = true;  // marca que a lista foi alterada
}