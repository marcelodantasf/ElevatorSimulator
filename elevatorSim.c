#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef char *String;
typedef struct andar andar;
typedef struct elevador elevador;
typedef struct chamadas chamadas;
typedef struct predio predio;

#define KEY "_"

typedef enum { SUBINDO, DESCENDO, PARADO } Direcao;

struct andar {
  int numero_andar;
  andar *anterior;
  andar *proximo;
};

struct chamadas {
  andar *destino;
  andar *origem;
  int tempo;
  Direcao direcao;
  struct chamadas *proxima;
};

struct elevador {
  andar *rotaCima;
  andar *rotaBaixo;
  andar *andar_atual;
  Direcao direcao;
  int inatividade;
  char percorrido[1000];
  int andares_percorridos;
  int deslocamento;
};

struct predio {
  elevador *elevadores;
  int num_elevadores;
  int num_andares;
  chamadas *chamadas;
};

void mostrarElevadores(elevador *elevador, int i) {
  printf("\n| E%d: ", i + 1);
  printf("%s\n", elevador->percorrido);
}

void irParaCima(elevador *elevador, int seg) {
  if (!elevador->rotaCima)
    return;

  andar *a = elevador->andar_atual;

  if (a->proximo && a->numero_andar < elevador->rotaCima->numero_andar) {
    elevador->andar_atual = a->proximo;
    printf("\n\nta no %d\n\n", elevador->andar_atual->numero_andar);
    elevador->andares_percorridos++;
  }

  if (elevador->andar_atual->numero_andar == elevador->rotaCima->numero_andar) {
    printf("\n\nta no %d\n\n", elevador->andar_atual->numero_andar);
    char cc[10];
    sprintf(cc, "%d", elevador->andar_atual->numero_andar);
    char ss[10];
    sprintf(ss, "%d", seg);
    char nova_string[50];
    snprintf(nova_string, sizeof(nova_string), "%s(%s) -> ", cc, ss);
    strcat(elevador->percorrido, nova_string);
    elevador->deslocamento = seg;
    andar *aux = elevador->rotaCima;
    elevador->rotaCima = elevador->rotaCima->proximo;
    free(aux);
  }

  if (!elevador->rotaCima && !elevador->rotaBaixo) {
    elevador->direcao = PARADO;
  }

  elevador->inatividade = 0;
}

void irParaBaixo(elevador *elevador, int seg) {
  if (!elevador->rotaBaixo)
    return;

  andar *a = elevador->andar_atual;

  if (a->anterior && a->numero_andar > elevador->rotaBaixo->numero_andar) {
    elevador->andar_atual = a->anterior;
    elevador->andares_percorridos++;
  }

  if (elevador->andar_atual->numero_andar ==
      elevador->rotaBaixo->numero_andar) {
    char cc[10];
    sprintf(cc, "%d", elevador->andar_atual->numero_andar);
    char ss[10];
    sprintf(ss, "%d", seg);
    char nova_string[50];
    snprintf(nova_string, sizeof(nova_string), "%s(%s) -> ", cc, ss);
    strcat(elevador->percorrido, nova_string);
    elevador->deslocamento = seg;
    andar *aux = elevador->rotaBaixo;
    elevador->rotaBaixo = elevador->rotaBaixo->proximo;
    free(aux);
  }

  if (!elevador->rotaCima && !elevador->rotaBaixo) {
    elevador->direcao = PARADO;
  }

  elevador->inatividade = 0;
}

void modoEsperar(elevador *elevador, int seg) {
  elevador->inatividade++;
  if (elevador->inatividade > 2 && elevador->andar_atual->numero_andar != 0) {
    elevador->andar_atual = elevador->andar_atual->anterior;

    if (elevador->andar_atual->numero_andar == 0) {
      elevador->direcao = PARADO;
      elevador->inatividade = 0;
    }
  }
}

void mover(elevador *elevador, int seg) {
  if (elevador->direcao == SUBINDO && elevador->rotaCima) {
    irParaCima(elevador, seg);
  } else if (elevador->direcao == DESCENDO && elevador->rotaBaixo) {
    irParaBaixo(elevador, seg);
  } else if (elevador->direcao == PARADO) {
    modoEsperar(elevador, seg);
  }
}

void moverElevadores(predio *predio, int seg) {
  int andares = 0;
  int deslocas = 0;
  for (int i = 0; i < predio->num_elevadores; i++) {
    andares += predio->elevadores[i].andares_percorridos;
    deslocas += predio->elevadores[i].deslocamento;
    mostrarElevadores(&predio->elevadores[i], i);
    mover(&predio->elevadores[i], seg);
  }
  printf("\n\n-- Resumo do conjunto de elevadores --\n");
  printf("tempo de deslocamento dos elevadores: %d\n", deslocas);
  printf("número de andares percorrido pelos elevadores: %d\n", andares);
}

void inicializar(struct predio *predio, int num_andares, int num_elevadores) {
  predio->num_andares = num_andares;
  predio->num_elevadores = num_elevadores;
  predio->elevadores = malloc(num_elevadores * sizeof(elevador));

  if (!predio->elevadores) {
    printf("Erro ao alocar memória para elevadores\n");
    return;
  }

  for (int i = 0; i < num_elevadores; i++) {
    predio->elevadores[i].rotaCima = NULL;
    predio->elevadores[i].rotaBaixo = NULL;
    predio->elevadores[i].andares_percorridos = 0;
    predio->elevadores[i].andar_atual = malloc(sizeof(andar) * num_andares);
    predio->elevadores[i].direcao = PARADO;
    predio->elevadores[i].inatividade = 0;

    if (!predio->elevadores[i].andar_atual) {
      printf("Erro ao alocar memória para andares\n");
      return;
    }

    for (int j = 0; j < num_andares; j++) {
      predio->elevadores[i].andar_atual[j].numero_andar = j;
      predio->elevadores[i].andar_atual[j].proximo =
          (j < num_andares - 1) ? &predio->elevadores[i].andar_atual[j + 1]
                                : NULL;
      predio->elevadores[i].andar_atual[j].anterior =
          (j > 0) ? &predio->elevadores[i].andar_atual[j - 1] : NULL;
    }

    predio->elevadores[i].andar_atual = &predio->elevadores[i].andar_atual[0];
  }
}

int escolherElevador(predio *predio, andar *novo_andar) {
  int melhorElevador = -1;
  int numAndares = predio->num_andares;

  for (int i = 0; i < predio->num_elevadores; i++) {
    elevador *e = &predio->elevadores[i];
    int distancia =
        abs(e->andar_atual->numero_andar - novo_andar->numero_andar);

    if (e->direcao == PARADO ||
        (e->direcao == SUBINDO &&
         novo_andar->numero_andar > e->andar_atual->numero_andar) ||
        (e->direcao == DESCENDO &&
         novo_andar->numero_andar < e->andar_atual->numero_andar)) {

      if (distancia < numAndares) {
        numAndares = distancia;
        melhorElevador = i;
      }
    }
  }

  return melhorElevador;
}
void inserirNaRota(predio *predio, andar *a, elevador *el) {
  if (el) {
    elevador *elevador = el;
    andar *novo_andar = malloc(sizeof(andar));
    if (!novo_andar) {
      printf("Erro ao alocar memória para o andar\n");
      return;
    }
    *novo_andar = *a;
    novo_andar->proximo = NULL;
    if (elevador->direcao == SUBINDO) {
      if (!elevador->rotaCima) {
        elevador->rotaCima = novo_andar;
      } else {
        andar *aux = elevador->rotaCima;
        andar *prev = NULL;
        while (aux && aux->numero_andar < novo_andar->numero_andar) {
          prev = aux;
          aux = aux->proximo;
        }
        if (prev) {
          novo_andar->proximo = aux;
          prev->proximo = novo_andar;
        } else {
          novo_andar->proximo = elevador->rotaCima;
          elevador->rotaCima = novo_andar;
        }
      }
    } else {
      if (!elevador->rotaBaixo) {
        elevador->rotaBaixo = novo_andar;
      } else {
        andar *aux = elevador->rotaBaixo;
        andar *prev = NULL;
        while (aux && aux->numero_andar > novo_andar->numero_andar) {
          prev = aux;
          aux = aux->proximo;
        }
        if (prev) {
          novo_andar->proximo = aux;
          prev->proximo = novo_andar;
        } else {
          novo_andar->proximo = elevador->rotaBaixo;
          elevador->rotaBaixo = novo_andar;
        }
      }
    }
    return;
  } else {
    int elevadorIndex = escolherElevador(predio, a);
    if (elevadorIndex == -1) {
      printf("Nenhum elevador disponível para a chamada.\n");
      return;
    }

    elevador *elevador = &predio->elevadores[elevadorIndex];

    andar *novo_andar = malloc(sizeof(andar));
    if (!novo_andar) {
      printf("Erro ao alocar memória para o andar\n");
      return;
    }
    *novo_andar = *a;
    novo_andar->proximo = NULL;

    if (elevador->direcao == PARADO) {
      if (novo_andar->numero_andar > elevador->andar_atual->numero_andar) {
        elevador->direcao = SUBINDO;
      } else if (novo_andar->numero_andar <
                 elevador->andar_atual->numero_andar) {
        elevador->direcao = DESCENDO;
      }
    }

    if (elevador->direcao == SUBINDO) {
      if (!elevador->rotaCima) {
        elevador->rotaCima = novo_andar;
      } else {
        andar *aux = elevador->rotaCima;
        andar *prev = NULL;
        while (aux && aux->numero_andar < novo_andar->numero_andar) {
          prev = aux;
          aux = aux->proximo;
        }
        if (prev) {
          novo_andar->proximo = aux;
          prev->proximo = novo_andar;
        } else {
          novo_andar->proximo = elevador->rotaCima;
          elevador->rotaCima = novo_andar;
        }
      }
    } else if (elevador->direcao == DESCENDO) {
      if (!elevador->rotaBaixo) {
        elevador->rotaBaixo = novo_andar;
      } else {
        andar *aux = elevador->rotaBaixo;
        andar *prev = NULL;
        while (aux && aux->numero_andar > novo_andar->numero_andar) {
          prev = aux;
          aux = aux->proximo;
        }
        if (prev) {
          novo_andar->proximo = aux;
          prev->proximo = novo_andar;
        } else {
          novo_andar->proximo = elevador->rotaBaixo;
          elevador->rotaBaixo = novo_andar;
        }
      }
    }
  }
}

char *overwriteComma(char *str) {
  char *newString = malloc(sizeof(char) * strlen(str));
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == ',')
      newString[i] = '_';
    else
      newString[i] = str[i];
  }
  return newString;
}

//!(to fix): função precisa ser refatorada,
//'T' precisa inserir função no tempo, 'En' deve "resetar" o status do elevador
// n e as chamadas devem ser oriundas desta função.
void decode(char str[], predio *p) {
  str = overwriteComma(str);

  if (str[0] == 'A') { // setar os andares a partir do maximo
    char *newString = strtok(str, KEY);
    newString = strtok(NULL, KEY);

    int num_andares = atoi(newString);
    printf("floor level read: %d\n", num_andares);

    inicializar(p, num_andares, 3);

    return;
  }

  if (str[0] == 'E') { // setar status do elevador

    char *newString = strtok(str, KEY);

    int numeroElevador = atoi(newString); // le o numero do elevador

    if (numeroElevador < 0 || numeroElevador > p->num_elevadores) {
      printf("Número de elevador inválido\n");
      return;
    }

    elevador *elevador_atual = &p->elevadores[numeroElevador];

    newString = strtok(NULL, KEY);
    int andar_inicio = atoi(newString);

    elevador_atual->andar_atual = &elevador_atual->andar_atual[andar_inicio];

    newString = strtok(NULL, KEY);
    Direcao direction = (strcmp(newString, "S") == 0) ? SUBINDO : DESCENDO;

    elevador_atual->direcao = direction;

    newString = strtok(NULL, KEY);

    int floors[10]; // buffer para os andares
    int count = 0;

    while (newString != NULL) {

      if (newString[0] == 'T')
        floors[count] = 0;
      else
        floors[count] = atoi(newString);

      newString = strtok(NULL, KEY);
      count++;
    }

    chamadas *novas_chamadas = malloc(count * sizeof(chamadas));

    for (int i = 0; i < count; i++) {
      printf("[%d]\n", floors[i]);

      andar *destino = &p->elevadores[numeroElevador].andar_atual[floors[i]];
      andar *origem;

      if (i < 1) {
        origem = elevador_atual->andar_atual;
      } else {
        origem = &p->elevadores[numeroElevador].andar_atual[floors[i - 1]];
      }

        novas_chamadas[i].origem = origem;
        novas_chamadas[i].destino = destino;
        novas_chamadas[i].tempo = 0;
        novas_chamadas[i].proxima = NULL;

        if(i > 0)
            novas_chamadas[i-1].proxima = &novas_chamadas[i];
      //inserirNaRota(p, destino, elevador_atual);

       if(!p->chamadas)
         p->chamadas = novas_chamadas;
      else {
        chamadas* aux = p->chamadas;
        while(aux->proxima)
          aux = aux->proxima;

        aux->proxima = novas_chamadas;
      }
    }

    return;
  }

       if (str[0] == 'T') { //adicionar chamada

           char *newString = strtok(str, KEY);
           int tempoNo = atoi(newString + 1);

           newString = strtok(NULL, KEY);
           printf("pessoa: %s\n", newString);

           newString = strtok(NULL, KEY);
          Direcao direction = (strcmp(newString, "S") == 0) ? SUBINDO : DESCENDO;  //

           newString = strtok(NULL, KEY);
           int origem = atoi(newString);

           newString = strtok(NULL, KEY);
           int destino = atoi(newString);

           chamadas *nova_chamada = malloc(sizeof(chamadas));
           nova_chamada->direcao = direction;
           nova_chamada->tempo = tempoNo;

           nova_chamada->origem = &p->elevadores->andar_atual[origem];

           nova_chamada->destino = &p->elevadores->andar_atual[destino];

           nova_chamada->proxima = NULL;

           // inserir a nova chamada na lista de chamadas do prédio
           if (p->chamadas == NULL) {
               p->chamadas = nova_chamada;
           } else {
               chamadas *aux = p->chamadas;
               while (aux->proxima != NULL) {
                   aux = aux->proxima;
               }
               aux->proxima = nova_chamada;
           }

           return;
       }

  printf("Formato invalido, digite as instrucoes conforme as regras\n");
}

void verificarChamadas(predio *predio, int segundoAtual) {
  if (predio->chamadas == NULL)
    return;
  chamadas *aux = predio->chamadas;

  while (aux->proxima) {
    if (aux->tempo == segundoAtual) {
      int elevadorIndex = escolherElevador(predio, aux->origem);
      elevador *e = &predio->elevadores[elevadorIndex];
      inserirNaRota(predio, aux->origem, e);
      inserirNaRota(predio, aux->destino, e);
    }
    aux = aux->proxima;
  }
}

int main() {
  struct predio meuPredio;
  // inicializar(&meuPredio, 20, 3);

    char* db[18] = {"AM_25", "E1_04_S_6,9,8", "E2_11_D_5,8,9,3,2,T", "E3_20_D_5,8,T,9",
                    "T5_P01_S_4_6", "T9_P02_S_5_10", "T10_P03_S_3_9", "T10_P05_S_8_15",
                    "T12_P01_D_6_4", "T15_P06_D_9_2", "T15_P07_S_2_13", "T18_P08_D_8_T",
                    "T21_P01_D_16_3", "T21_P10_S_T_13", "T21_P11_S_T_12", "T23_P12_S_T_15",
                    "T28_P13_S_2_13"};

  andar andares[6] = {{1, NULL, NULL}, {2, NULL, NULL}, {3, NULL, NULL},
                      {4, NULL, NULL}, {5, NULL, NULL}, {15, NULL, NULL}};

  char *cmd2 = "E1_01_S_6,9,8";
 // decode(cmd2, &meuPredio);

 // char *cmd3 = "E2_01_S_5,7,10";
  //decode(cmd3, &meuPredio);

  decode(db[0], &meuPredio);

  //decode(cmd2,&meuPredio);

  decode("T5_P01_S_4_6",&meuPredio);

   inserirNaRota(&meuPredio, &andares[5], NULL);
   inserirNaRota(&meuPredio, &andares[1], NULL);

  int segundos = 0;
  while (1) {
    system("clear");
    segundos++;
    printf("\n\nSegundo: %d\n\n", segundos - 1);
    verificarChamadas(&meuPredio, segundos - 1);
    moverElevadores(&meuPredio, segundos);
    sleep(1);
  }

  return 0;
}