#include "DoublyLinkedList_NursingCall.h"  // Inclui o header da lista
#include <cstring>                         // para strncpy


/* Construtor:
    Inicializa lista vazia (NULL) */
List_NursingCall::List_NursingCall() {
  head = nullptr;
  tail = nullptr;
  current = nullptr;
  total = 0;
}


/* Destrutor:
    - chama clear() para liberar memória dos nós antes de o objeto ser destruído 
    - previne vazamentos se a lista ainda tiver nós */
List_NursingCall::~List_NursingCall() {
  clear();
}


// =========================
//        Consultas
// =========================
int List_NursingCall::getTotal() {  // retorna o número de nós
  return total;
}

// retorna o campo infirmary (texto) do nó current ou nullptr se não houver
const char* List_NursingCall::getInfirmaryCurrent() {
  if (current != nullptr) return current->infirmary;
  return nullptr;
}

// retorna o campo ID do nó current
const char* List_NursingCall::getIdCurrent() {
  if (current) return current->id;
  return nullptr;
}

// retorna true quando há pelo menos um nó
bool List_NursingCall::hasNursingCall() {
  return total > 0;
}

// Validação da enfermaria
bool List_NursingCall::isValidInfirmary(const char* infirmary) {
  if (infirmary == nullptr || infirmary[0] == '\0') return false;
  return true;
}

// Função auxiliar que define o valor de "doNotRemoveCurrent" que sinaliza se pode ou não remover o current se atingir o limite
void List_NursingCall::setDoNotRemoveCurrent(bool value) {
  doNotRemoveCurrent = value;
}
// Função auxiliar que retorna o valor de "getDoNotRemoveCurrent"
bool List_NursingCall::getDoNotRemoveCurrent() const {
  return doNotRemoveCurrent;
}


// =========================
//        Add Nó
// =========================
bool List_NursingCall::add(const char* infirmary, const char* id) {  // Adicionar um nó ao final da lista
  // verifica se o ID do dispositivo é valido
  if (id == nullptr || id[0] == '\0') return false;

  // verifica se o número do quarto é valido
  if (!isValidInfirmary(infirmary)) return false;

  // gerencia o limite de memória
  if (total >= MAX_CALLS && !doNotRemoveCurrent) {  // Verifica se lista atingiu capacidade máxima
    // loop: apaga todos os elementos que atingiram o limite se não estiver na tela de confirmação de exclusão
    while (total >= MAX_CALLS) {
      if (!removeOldestCall()) return false;  // se atingiu o limite, tenta remover chamada mais antiga (política FIFO)
    }
  }

  NursingCall* new_node = new NursingCall;  // Cria um novo nó
  if (new_node == nullptr) return false; // verifica se alocação foi bem-sucedida

  // Copia a enfermaria
  strncpy(new_node->infirmary, infirmary, sizeof(new_node->infirmary) - 1);
  new_node->infirmary[sizeof(new_node->infirmary) - 1] = '\0';
  
  // Copia o ID
  strncpy(new_node->id, id, sizeof(new_node->id) - 1);
  new_node->id[sizeof(new_node->id) - 1] = '\0';

  new_node->next = nullptr;  // Como vai ser o último nó, o campo next é vazio
  new_node->prev = tail;     // Liga o prev do novo ao tail atual

  if (tail != nullptr) {  // Se havia um tail, ajusta tail->next
    tail->next = new_node;
  } else {  // senão (lista vazia) ajusta head
    head = new_node;
    // define current como o novo nó
    current = new_node;
  }

  // Atualiza tail com o novo nó
  tail = new_node;
  // Incrementa total
  total++;
  return true;  // elemento adicionado com sucesso
}


// =========================
//        Navegações
// =========================
void List_NursingCall::next() {  // Move current para próximo nó
  // Se o current ou o próximo nó de current não for vazio
  if (current != nullptr && current->next != nullptr) {
    current = current->next;
  }
}

void List_NursingCall::prev() {  // Move current para o nó anterior
  // Se o current ou o nó anterior de current não for vazio
  if (current != nullptr && current->prev != nullptr) {
    current = current->prev;
  }
}


// =========================
//        Remoção
// =========================

bool List_NursingCall::remove(NursingCall* node) {

  if (!hasNursingCall()) {
    return false;
  }

  if (node == nullptr) return false;
  /* ==============================
        Ajustar o ponteiro anterior
     ==============================*/
  // Se existe um nó anterior, faz esse nó apontar (next) para o próximo de temp
  if (node->prev != nullptr) {
    node->prev->next = node->next;
  } else {  // Caso contrário - node é o primeiro da lista
    // Atualiza head para ser o próximo nó
    head = node->next;
  }

  /* ==============================
        Ajustar o ponteiro seguinte
       ==============================*/
  if (node->next != nullptr) {
    // Se existe um nó seguinte - faz esse nó apontar (prev) para o anterior de node
    node->next->prev = node->prev;
    // Atualiza current para esse próximo nó

    if (current == node) {
      current = node->next;
    }
  } else {  // Caso contrário - node é último da lista
    // Atualiza tail para o anterior de node
    tail = node->prev;
    // Atualiza current para esse anterior

    if (current == node) {
      current = node->prev;
    }
  }

  // Remove da memória o nó que foi desconectado, evitando vazamento de memória
  delete node;
  // Decrementa o número de nós da lista
  total--;
  return true;
}


bool List_NursingCall::removeCurrent() {

  // variável auxiliar para segurar o nó que será removido
  NursingCall* temp = current;

  return remove(temp);

}


// Método auxiliar para remover elemento mais antigo (primeiro)
bool List_NursingCall::removeOldestCall() {
  if (head == nullptr) return false;  // Verifica se lista está vazia (Se vazia: retorna false (nada para remover))

  // Salva referência ao elemento a ser removido
  NursingCall* toRemove = head;

  // Ajusta head para apontar para o segundo elemento
  head = head->next;  // Se havia apenas 1 elemento: head se torna nullptr

  if (head != nullptr) {  // caso a lista não esteja vazia
    // Remove link anterior do novo primeiro
    head->prev = nullptr;
  } else {
    // Lista ficou vazia atualiza tail para nullptr
    tail = nullptr;
  }

  // Se current apontava para elemento removido
  if (current == toRemove) {
    current = head;  // Redireciona current para o novo head
  }

  delete toRemove;  // Libera memória do elemento removido
  total--;          // Decrementa contador de elementos
  return true;      // Retorna sucesso - elemento removido com sucesso
}


bool List_NursingCall::removalById(const char* id) {

  NursingCall* node = head;

  while (node != nullptr) {
    if (strcmp(node->id, id)==0) {
      //remove no da liista
      remove(node);
      return true;
    }
    node = node->next;
  }
  return false;
}


// Limpar toda a lista
void List_NursingCall::clear() {
  // Cria um ponteiro auxiliar - começa apontando para o primeiro nó da lista
  NursingCall* temp = head;
  while (temp != nullptr) {  // enquanto ainda houver nós na lista
    // Salva o endereço do próximo nó antes de apagar o nó atual
    NursingCall* prox = temp->next;
    // Deletando o nó atual
    delete temp;
    // Faz temp apontar para o próximo nó salvo em prox
    temp = prox;
  }
  // Reseta a lista
  head = nullptr;
  tail = nullptr;
  current = nullptr;
  total = 0;
}