#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ---------------------------------------------------

// typedefs das estuturas que o usuario irá digitar
typedef struct Data {
  int dia;
  int mes;
  int ano;
} Data;

typedef struct Registro{ 
  char nome[100];
  int idade;
  char rg[10];
  struct Data *entrada;
} Registro;

// ---------------------------------------------------

// LDE - Lista Dinâmica Encadeada
typedef struct Elista{ // Celula
  Registro *dados; // valor
  struct Elista *proximo; // usa o struct pq ela ainda nao existe
} Elista;

typedef struct Lista { // LDE
  Elista *inicio; // primeiro
  int qtde;
} Lista;

Lista *cria_lista() { // LDE é o tipo de dado
  Lista *lista = malloc(sizeof(Lista)); // alocar a memoria
  lista->inicio = NULL;
  lista->qtde = 0;
  return lista;
}

Elista *cria_celula(Registro *registro) {
  Elista *celula = malloc(sizeof(Elista));
  celula->proximo = NULL;
  celula->dados = registro;
  return celula;
}

Data *cria_data(int dia, int mes, int ano) {
  Data *data = malloc(sizeof(Data));
  data->dia = dia;
  data->mes = mes;
  data->ano = ano;
  return data;
}

Registro *cria_registro(char nome[], int idade, char rg[], int dia, int mes, int ano) {
  Data *data = cria_data(dia, mes, ano);
  Registro *registro = malloc(sizeof(Registro));
  strcpy(registro->nome, nome);
  registro->idade = idade;
  strcpy(registro->rg, rg);
  registro->entrada = data;
  return registro;
}

void inserir_lde(Lista *lista, char nome[], int idade, char rg[], int dia, int mes, int ano) {
  Registro *registro = cria_registro(nome, idade, rg, dia, mes, ano);
  Elista *novo = cria_celula(registro);
  novo->dados = registro;
  if(lista->qtde == 0) {
    lista->inicio = novo;
  } else {
    novo->proximo = lista->inicio;
    lista->inicio = novo;
  }
  lista->qtde++;
}

void remover_lde(Lista *l, char rg[10]) {
  Elista *atual = l->inicio;
  Elista *anterior = NULL;

  while(atual != NULL && (strcmp(atual->dados->rg, rg) != 0)) {
      anterior = atual;
      atual = atual->proximo;
  }

  if(atual == NULL) {
    printf("RG do paciente não encontrado\n\n");
    return;
  }

  if(anterior == NULL && atual->proximo == NULL) { // unitario
    l->inicio = NULL;
    return;
  }

  if(anterior == NULL) { // inicio
    l->inicio = atual->proximo;
    return;
  }

  if(atual->proximo == NULL) { // fim
    anterior->proximo = NULL;
    return;
  }

  if(anterior != NULL && atual->proximo != NULL) { // meio
    anterior->proximo = atual->proximo;
    return;
  }
  l->qtde--;
  free(atual);
}

void atualiza_dados(Lista *lista, char rg[10]) {
  Elista *atual = lista->inicio;
  while(atual != NULL) {
    if(strcmp(atual->dados->rg, rg) == 0) {
      printf("Digite o novo nome: ");
      scanf("%s", atual->dados->nome);
      scanf("");
      printf("Digite a nova idade: ");
      scanf("%d", &atual->dados->idade);
      printf("Digite o novo RG: ");
      scanf("%s", atual->dados->rg);
      printf("Digite o novo dia de entrada: ");
      scanf("%d", &atual->dados->entrada->dia);
      printf("Digite o novo mês de entrada: ");
      scanf("%d", &atual->dados->entrada->mes);
      printf("Digite o novo ano de entrada: ");
      scanf("%d", &atual->dados->entrada->ano);
      return;
    }
    else {
      atual = atual->proximo;
    }
  }
}

void consulta_paciente(Lista *lista, char rg[10]) {
  Elista *atual = lista->inicio;
  if(atual != NULL) {
    if(strcmp(atual->dados->rg, rg) == 0) {
      printf("Nome: %s\n", atual->dados->nome);
      printf("Idade: %d\n", atual->dados->idade);
      printf("RG: %s\n", atual->dados->rg);
      printf("Dia de entrada: %d\n", atual->dados->entrada->dia);
      printf("Mês de entrada: %d\n", atual->dados->entrada->mes);
      printf("Ano de entrada: %d\n", atual->dados->entrada->ano);
      printf("\n");
    }
  }
}

void mostrar_lde(Lista *lista) {
  Registro registro;
  Elista *atual = lista->inicio;
  while(atual != NULL) {
    printf("Nome: %s", atual->dados->nome);
    printf("\nIdade: %d", atual->dados->idade);
    printf("\nRG: %s ", atual->dados->rg);
    printf("\nDia: %d ", atual->dados->entrada->dia);
    printf("\nMês: %d ", atual->dados->entrada->mes);
    printf("\nAno: %d", atual->dados->entrada->ano);
    printf("\n-----------------------------------\n");
    atual = atual->proximo;
  }
  printf("\n");
}

void cadastrar_paciente(Lista *lista) {
  int idade, dia, mes, ano;
  char nome[100], rg[10];
  printf("----- CADASTRAR NOVO PACIENTE -----");
  printf("\nNome:");
  setbuf(stdin, NULL);
  scanf("%99[^\n]", nome);
  setbuf(stdin, NULL);
  printf("Idade:");
  scanf("%d", &idade);
  setbuf(stdin, NULL);
  printf("RG:");
  scanf("%9[^\n]", rg);
  setbuf(stdin, NULL);
  printf("Dia:");
  scanf("%d", &dia);
  printf("Mês:");
  scanf("%d", &mes);
  printf("Ano:");
  scanf("%d", &ano);
  inserir_lde(lista, nome, idade, rg, dia, mes, ano);
  printf("Paciente cadastrado com sucesso!\n\n");
}

// ---------------------------------------------------

// Fila
typedef struct Efila{ // Celula
  Registro *dados; // valor
  struct Efila *proximo; // usa o struct pq ela ainda nao existe
  struct Efila *anterior;
} Efila;

typedef struct {
  Efila *head;
  Efila *tail;
  int qtde;
} Fila;

Efila *cria_efila(Registro *registro){
  Efila *celula = malloc(sizeof(Efila));
  celula->proximo = NULL;
  celula->anterior = NULL;
  celula->dados = registro;
  return celula;
}

Fila *cria_queue(){
  Fila *queue = malloc(sizeof(Fila));
  queue->head = NULL;
  queue->tail = NULL;
  queue->qtde = 0;
  return queue;
}

void enqueue(Fila *queue, Lista *lista, char rg[10]){
  Elista *atual = lista->inicio;
  while(atual != NULL) {
    if(strcmp(atual->dados->rg, rg) == 0) {
      Efila *novo = cria_efila(atual->dados);
      if(queue->qtde == 0) {
        queue->head = novo;
      } else {
        novo->anterior = queue->tail;
        queue->tail->proximo = novo;
      }
      queue->tail = novo;
      queue->qtde++;
    }
    atual = atual->proximo;
  }
}

void dequeue(Fila *queue){
  if(queue->qtde != 0) {
        Registro *valor = queue->head->dados;
        Efila *liberar = queue->head;
    if(queue->qtde == 1) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = queue->head->proximo;
        queue->head->anterior = NULL;
    }
    queue->qtde--;
    printf("\nPaciente removido: %s\n", liberar->dados->nome);
    free(liberar);
    } else {
        printf("Fila vazia!\n");
    }
}

void mostrar_fila(Fila *queue){
  Efila *atual = queue->head;
    while(atual != NULL) {
      printf("Nome: %s", atual->dados->nome);
      printf("\nIdade: %d", atual->dados->idade);
      printf("\nRG: %s ", atual->dados->rg);
      printf("\nDia: %d ", atual->dados->entrada->dia);
      printf("\nMês: %d ", atual->dados->entrada->mes);
      printf("\nAno: %d", atual->dados->entrada->ano);
      printf("\n-----------------------------------\n");
      atual = atual->proximo;
    }
}

// ---------------------------------------------------

// Árvore Binária de Busca
typedef struct EABB {
    Registro *dados; // valor
    struct EABB *esq;
    struct EABB *dir;
    struct EABB *pai;
} EABB;

typedef struct ABB {
    EABB *raiz;
    int qtde;
} ABB;

EABB *cria_vertice(Elista *paciente) {
    EABB *novo = malloc(sizeof(EABB));
    novo->dir = NULL;
    novo->esq = NULL;
    novo->pai = NULL;
    novo->dados = paciente->dados;
    return novo;
}

ABB *cria_arvore() {
    ABB *arvore = malloc(sizeof(ABB));
    arvore->raiz = NULL;
    arvore->qtde = 0;
    return arvore;
}

void in_ordem(EABB *raiz) { // printar em ordem
  if (raiz != NULL) {
    in_ordem(raiz->esq);
    printf("---------------------");
    printf("\nNome: %s", raiz->dados->nome);
    printf("\nIdade: %d", raiz->dados->idade);
    printf("\nRG: %s ", raiz->dados->rg);
    printf("\nDia: %d ", raiz->dados->entrada->dia);
    printf("\nMês: %d ", raiz->dados->entrada->mes);
    printf("\nAno: %d\n", raiz->dados->entrada->ano);
    in_ordem(raiz->dir);
    }
}

void inserir_arvore_dia(ABB *arvore, Elista *paciente) {
    EABB *novo = cria_vertice(paciente); // 1
    // Se a ávore estiver vazia 
    if(arvore->qtde == 0){ // 2
        arvore->raiz = novo;// Novo vértice é a raiz da árvore
        arvore->qtde++;
    }else{ // Árvore não vazia // 3
        EABB *atual = arvore->raiz;
        EABB *anterior = NULL;
        while(atual != NULL){ //4
            anterior = atual;
            if(paciente->dados->entrada->dia < atual->dados->entrada->dia){
                atual = atual->esq;
            }else{
                atual = atual->dir;
            }
        }
        if(paciente->dados->entrada->dia < anterior->dados->entrada->dia){ // 5
            anterior->esq = novo;
        }else{
            anterior->dir = novo;
        }
        novo->pai = anterior;
    }
}

void inserir_arvore_mes(ABB *arvore, Elista *paciente) {
    EABB *novo = cria_vertice(paciente); // 1
    // Se a ávore estiver vazia 
    if(arvore->qtde == 0){ // 2
        arvore->raiz = novo;// Novo vértice é a raiz da árvore
        arvore->qtde++;
    }else{ // Árvore não vazia // 3
        EABB *atual = arvore->raiz;
        EABB *anterior = NULL;
        while(atual != NULL){ //4
            anterior = atual;
            if(paciente->dados->entrada->mes < atual->dados->entrada->mes){
                atual = atual->esq;
            }else{
                atual = atual->dir;
            }
        }
        if(paciente->dados->entrada->mes < anterior->dados->entrada->mes){ // 5
            anterior->esq = novo;
        }else{
            anterior->dir = novo;
        }
        novo->pai = anterior;
    }
}

void inserir_arvore_ano(ABB *arvore, Elista *paciente) {
    EABB *novo = cria_vertice(paciente); // 1
    // Se a ávore estiver vazia 
    if(arvore->qtde == 0){ // 2
        arvore->raiz = novo;// Novo vértice é a raiz da árvore
        arvore->qtde++;
    }else{ // Árvore não vazia // 3
        EABB *atual = arvore->raiz;
        EABB *anterior = NULL;
        while(atual != NULL){ //4
            anterior = atual;
            if(paciente->dados->entrada->ano < atual->dados->entrada->ano){
                atual = atual->esq;
            }else{
                atual = atual->dir;
            }
        }
        if(paciente->dados->entrada->ano < anterior->dados->entrada->ano){ // 5
            anterior->esq = novo;
        }else{
            anterior->dir = novo;
        }
        novo->pai = anterior;
    }
}

void inserir_arvore_idade(ABB *arvore, Elista *paciente) {
    EABB *novo = cria_vertice(paciente); // 1
    // Se a ávore estiver vazia 
    if(arvore->qtde == 0){ // 2
        arvore->raiz = novo;// Novo vértice é a raiz da árvore
        arvore->qtde++;
    }else{ // Árvore não vazia // 3
        EABB *atual = arvore->raiz;
        EABB *anterior = NULL;
        while(atual != NULL){ //4
            anterior = atual;
            if(paciente->dados->idade < atual->dados->idade){
                atual = atual->esq;
            }else{
                atual = atual->dir;
            }
        }
        if(paciente->dados->idade < anterior->dados->idade){ // 5
            anterior->esq = novo;
        }else{
            anterior->dir = novo;
        }
        novo->pai = anterior;
    }
}

void mostrar_ordem_dia(Lista *lista) {
  ABB *arvore = cria_arvore();
  Elista *atual = lista->inicio;
  for(int i = 0; i < lista->qtde; i++) {
    inserir_arvore_dia(arvore, atual);
    atual = atual->proximo;
  }
  in_ordem(arvore->raiz);
}

void mostrar_ordem_mes(Lista *lista) {
  ABB *arvore = cria_arvore();
  Elista *atual = lista->inicio;
  for(int i = 0; i < lista->qtde; i++) {
    inserir_arvore_mes(arvore, atual);
    atual = atual->proximo;
  }
  in_ordem(arvore->raiz);
}

void mostrar_ordem_ano(Lista *lista) {
  ABB *arvore = cria_arvore();
  Elista *atual = lista->inicio;
  for(int i = 0; i < lista->qtde; i++) {
    inserir_arvore_ano(arvore, atual);
    atual = atual->proximo;
  }
  in_ordem(arvore->raiz);
}

void mostrar_ordem_idade(Lista *lista) {
  ABB *arvore = cria_arvore();
  Elista *atual = lista->inicio;
  for(int i = 0; i < lista->qtde; i++) {
    inserir_arvore_idade(arvore, atual);
    atual = atual->proximo;
  }
  in_ordem(arvore->raiz);
}


void opcao5() {
  printf("----- SOBRE -----");
  printf("\nNome: Felipe Orlando Lanzara, RA: 24.122.055-7");
  printf("\nNome: Nathan Dantas Mendes, RA: 24.122.041-7");
  printf("\nCiclo: 4ª Semestre");
  printf("\nCurso: Ciência da Computação");
  printf("\nDisciplina: Estrutura de Dados");
  printf("\nData: 17/11/2023\n\n");
}

int main(void) {
  Lista *lista = cria_lista();
  Fila *fila = cria_queue();
  while(1) {
    printf("----- GERENCIADOR DE ATENDIMENTO MÉDICO -----");
    printf("\n1- CADASTRAR");
    printf("\n2- ATENDIMENTO");
    printf("\n3- PESQUISA");
    printf("\n4- CARREGAR / SALVAR");
    printf("\n5- SOBRE");
    printf("\n6- ENCERRAR PROGRAMA");
    printf("\n-> Escolha uma dentre as opções acima: ");
    int opcao;
    scanf("%d", &opcao);
    printf("\n");

    if(opcao == 1) {
      while(1) {
        printf("----- CADASTRO -----");
        printf("\n1- Cadastrar novo paciente"); // feito
        printf("\n2- Consultar paciente cadastrado");
        printf("\n3- Mostrar lista completa"); //feito
        printf("\n4- Atualizar dados de paciente"); // feito
        printf("\n5- Remover paciente"); // feito
        printf("\n6- Voltar ao menu alterior"); // feito
        printf("\n-> Escolha uma dentre as opções acima: ");
        int subopcao;
        scanf("%d", &subopcao);
        printf("\n");
        if(subopcao == 1) {
          cadastrar_paciente(lista);
          break;
        }
        else if(subopcao == 2) {
          char rg[10];
          printf("Digite o RG do paciente que deseja atualizar os dados: ");
          scanf("%s", rg);
          consulta_paciente(lista, rg);
        } 
        else if(subopcao == 3) {
          mostrar_lde(lista);
        }
        else if(subopcao == 4) {
          char rg[10];
          printf("Digite o RG do paciente que deseja atualizar os dados: ");
          scanf("%s", rg);
          atualiza_dados(lista, rg);
        }
        else if(subopcao == 5) {
          char rg[10];
          printf("Digite o RG do paciente que deseja remover: ");
          scanf("%s", rg);
          remover_lde(lista, rg);
        }
        else if(subopcao == 6) {
          break;
        }
        else {
          printf("\nOpção inválida, digite novamente.\n");
        }
      }
    }

    else if(opcao == 2) {
      while(1) {
        printf("----- ATENDIMENTO -----");
        printf("\n1- Enfileirar paciente");
        printf("\n2- Desenfileirar paciente");
        printf("\n3- Mostrar fila");
        printf("\n4- Voltar ao menu alterior");
        printf("\n-> Escolha uma dentre as opções acima: ");
        int subopcao;
        scanf("%d", &subopcao);
        printf("\n");
        if(subopcao == 1) {
          char rg[10];
          printf("Digite o RG do paciente que deseja enfileirar: ");
          setbuf(stdin, NULL);
          scanf("%9[^\n]", rg);
          setbuf(stdin, NULL);
          enqueue(fila, lista, rg);
        }
        else if(subopcao == 2) {
          dequeue(fila);
        }
        else if(subopcao == 3) {
          mostrar_fila(fila);
        }
        else if(subopcao == 4) {
          break;
        }
        else {
          printf("\nOpção inválida, digite novamente.\n");
        }
      }
    }

    else if(opcao == 3) {
      while(1) {
        printf("----- PESQUISA -----");
        printf("\n1- Mostrar registros ordenados por ano de registro");
        printf("\n2- Mostrar registros ordenados por mês de registro");
        printf("\n3- Mostrar registros ordenados por dia de registro");
        printf("\n4- Mostrar registros ordenados por idade do paciente");
        printf("\n5- Voltar ao menu alterior");
        printf("\n-> Escolha uma dentre as opções acima: ");
        int subopcao;
        scanf("%d", &subopcao);
        printf("\n");
        if(subopcao == 1) {
          mostrar_ordem_ano(lista);
        }
        else if(subopcao == 2) {
          mostrar_ordem_mes(lista);
        }
        else if(subopcao == 3) {
          mostrar_ordem_dia(lista);
        }
        else if(subopcao == 4) {
          mostrar_ordem_idade(lista);
        }
        else if(subopcao == 5) {
          break;
        }
        else {
          printf("\nOpção inválida, digite novamente.\n");
        }
      }
    }

    else if(opcao == 4) {
      printf("----- CARREGAR / SALVAR -----\n\n");
    }

    else if(opcao == 5) {
      opcao5();
    }

    else if(opcao == 6) {
      printf("----- ENCERRAR PROGRAMA -----");
      printf("\nPrograma encerrado!");
      break;
    }

    else {
      printf("Opção inválida, digite novamente.\n");
      printf("\n");
    }

  }
}