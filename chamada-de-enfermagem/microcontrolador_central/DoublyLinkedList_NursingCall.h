#ifndef DOUBLY_LINKED_LIST_NURSING_CALL_H
#define DOUBLY_LINKED_LIST_NURSING_CALL_H


// NursingCall: nó
struct NursingCall {
  char infirmary[32]; // identificador textual da enfermaria
  char id[32];        // id da chamada (tamanho fixo)
  NursingCall* next;  // ponteiro para próximo nó
  NursingCall* prev;  // ponteiro para nó anterior
};


class List_NursingCall {
  private:
    NursingCall* head;                 // primeiro nó
    NursingCall* tail;                 // último nó
    NursingCall* current;              // nó selecionado (para navegação e remoção)
    int total;                         // número de nós
    static const int MAX_CALLS = 300;  // limite máximo de chamadas (número máximo de NÓs na lista)

    // Flag que indica se pode ou não remover o current quando a lista atingir o limite de inserção.
    bool doNotRemoveCurrent = false;
    
    // verifica se a enfermaria é valida 
    bool isValidInfirmary(const char* infirmary);

    // Função que remove se atingir o limite (lista cheia)
    bool removeOldestCall();

    // remove no pelo ponteiro
    bool remove(NursingCall* node);

    // área pública que permite manipular e consultar a lista
  public:
    List_NursingCall();
    ~List_NursingCall();

    // Consultas
    int getTotal();
    // retorna o campo infirmary do nó current (texto) ou nullptr se não houver
    const char* getInfirmaryCurrent();
    const char* getIdCurrent();
    bool hasNursingCall();
    // define o valor da flag (true/false) que indica se pode ou não remover o current quando a lista atingir o limite de inserção
    void setDoNotRemoveCurrent(bool value);
    // retorna o estado atual da flag que indica se pode ou não remover o current quando a lista atingir o limite de inserção
    bool getDoNotRemoveCurrent() const;

    // Navegações
    void next();
    void prev();

    // Modificações
    bool add(const char* infirmary, const char* id);
    bool removeCurrent();
    bool removalById(const char* id);
    void clear();
};

#endif  // DOUBLY_LINKED_LIST_NURSING_CALL_H