#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef char* String;
typedef struct floor floor;
typedef struct elevator elevator;
typedef struct chamadas chamadas;
typedef struct building building;

#define KEY "_"

struct floor {
    int floor_number;
    floor *prev;
    floor *next;
};

struct chamadas {
    floor *destino;
    floor *origem;
    int tempo;
    bool dir;
    struct chamadas *prox;
};

struct building {
    elevator *elevators;
    int num_elevators;
    chamadas *calls;
};

struct elevator {
    floor *rotaSubindo;
    floor *rotaDescendo;
    floor *current_floor;
    floor *floors;
    bool direction; // true = subindo, false = descendo
};

void mostrarElevadores(elevator *elevador, int i) {
        printf("[E%d]\n", i + 1);
        printf("Direção atual: %s\n", elevador->direction ? "subindo" : "descendo");
    if(elevador->current_floor->floor_number == 0)
    {
         printf("Andar atual: térreo\n\n");
    }   
    else {
        printf("floor atual: %d\n\n", elevador->current_floor->floor_number);
    } 

    if (elevador->direction) {
        printf("Rota subindo: ");
        floor *f = elevador->rotaSubindo;
        while (f) {
            printf("%d ", f->floor_number);
            f = f->next;
        }
        printf("\n");
    } else {
        printf("Rota descendo: ");
        floor *f = elevador->rotaDescendo;
        while (f) {
            printf("%d ", f->floor_number);
            f = f->next;
        }
        printf("\n");
    }

    
}

void subir(elevator *elevador) {
    elevador->current_floor = elevador->current_floor->next;
    floor *aux = elevador->rotaSubindo;
    elevador->rotaSubindo = elevador->rotaSubindo->next;
    free(aux);

    if (!elevador->rotaSubindo) {
        elevador->direction = false;
    }
}

void descer(elevator *elevador) {
    while (elevador->current_floor &&
           elevador->current_floor->floor_number > elevador->rotaDescendo->floor_number) {

        elevador->current_floor = elevador->current_floor->prev;
    }

    floor *aux = elevador->rotaDescendo;
    elevador->rotaDescendo = elevador->rotaDescendo->next;
    free(aux);

    if (!elevador->rotaDescendo) {
        elevador->direction = true;
    }
}

void move(elevator *elevador, int num_elevador) {
    if(elevador->rotaSubindo) {
      subir(elevador);
    }

   if(elevador->rotaDescendo) {
        descer(elevador);
    }
}

void moverElevadores(building *predio) {
    for (int i = 0; i < predio->num_elevators; i++) {
        mostrarElevadores(&predio->elevators[i], i);
        move(&predio->elevators[i], i);
    }
}

void init(struct building *predio, int num_floors, int num_elevators) {
    predio->num_elevators = num_elevators;
    predio->elevators = malloc(num_elevators * sizeof(elevator));

    if (!predio->elevators) {
        printf("Erro ao alocar memória para elevadores\n");
        return;
    }

    for (int i = 0; i < num_elevators; i++) {
        predio->elevators[i].rotaSubindo = NULL;
        predio->elevators[i].rotaDescendo = NULL;
        predio->elevators[i].floors = malloc(num_floors * sizeof(floor));
        predio->elevators[i].direction = true;

        if (!predio->elevators[i].floors) {
            printf("Erro ao alocar memória para andares\n");
            return;
        }

        for (int j = 0; j < num_floors; j++) {
            predio->elevators[i].floors[j].floor_number = j;
            predio->elevators[i].floors[j].next = (j < num_floors - 1) ? &predio->elevators[i].floors[j + 1] : NULL;
            predio->elevators[i].floors[j].prev = (j > 0) ? &predio->elevators[i].floors[j - 1] : NULL;
        }

        predio->elevators[i].current_floor = &predio->elevators[i].floors[0];
    }
}

void insertOnRoute(elevator *elevador, floor *andar) {
    floor *novo = malloc(sizeof(floor));
    if (!novo) {
        printf("Erro ao alocar memória para o andar\n");
        return;
    }
    *novo = *andar;

    if (elevador->direction) {
        if (!elevador->rotaSubindo) {
            elevador->rotaSubindo = novo;
        } else {
            floor *aux = elevador->rotaSubindo;
            while (aux->next && aux->next->floor_number < novo->floor_number) {
                aux = aux->next;
            }
            novo->next = aux->next;
            aux->next = novo;
        }
    } else {
        if (!elevador->rotaDescendo) {
            elevador->rotaDescendo = novo;
        } else {
            floor *aux = elevador->rotaDescendo;
            while (aux->next && aux->next->floor_number > novo->floor_number) {
                aux = aux->next;
            }
            novo->next = aux->next;
            aux->next = novo;
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

/*void receive(char str[], building *p) {

  if (str[0] == 'A') {
    char *newString = strtok(str, KEY);
    newString = strtok(NULL, KEY);
    printf("string after decode: %s\n", newString);

    floor *recebido = (floor *)malloc(sizeof(floor));
    recebido->floor_number = atoi(newString);

    printf("floor level read: %d\n", recebido->floor_number);

    p->maximo = recebido;

    printf("max floor level: %d\n\n", p->maximo->id);
    return;
  }

  if (str[0] == 'E') {
    printf("INPUT: %s\n", str);

    str = overwriteComma(str);

    char *newString = strtok(str, KEY);
    printf("%s\n", newString);

    int elevatorNo = atoi(newString + 1);
    printf("elevator read: %d\n", elevatorNo);

    p->elevators[0] = malloc(sizeof(elevator));

    newString = strtok(NULL, KEY);
    printf("%s\n", newString);

    int currentFloor = atoi(newString);
    printf("current floor read: %d\n", currentFloor);

    floor *current = malloc(sizeof(floor));
    current->floor_number = currentFloor;

    newString = strtok(NULL, KEY);
    printf("%s\n", newString);

    bool way = (strcmp(newString, "S") == 0) ? true : false;
    printf("direction read: %d\n", way);

    newString = strtok(NULL, KEY);
    printf("%s\n", newString);

    int count = 0;
    char *backup = newString;

    while (newString != NULL) {
      count++;
      newString = strtok(NULL, KEY);
    }
    printf("count: %d\n", count);

    int floors[3];
    int i = 0;
    int n = 0;

    while (n < count) {
      if (backup[i] != '_') {
        if (atoi(&backup[i]) == 0)
          i++;
        floors[n] = atoi(&backup[i]);
        printf("[%d]\n", floors[n]);
        n++;
      }
      i++;
    }

    chamadas *calls = malloc(sizeof(chamadas) * count);

    for (int i = 0; i < count; i++) {
      calls[i].dir = way;
      calls[i].tempo = 0;

      if (i < 1) {
        calls[i].origem = current;
        // inserir mecanismo de busca de floor
        calls[i].destino = malloc(sizeof(floor));
        calls[i].destino->floor_number = floors[i];
      } else {
        calls[i].origem = calls[i - 1].destino;
        calls[i].destino = malloc(sizeof(floor));
        calls[i].destino->floor_number = floor[i];
      }
    }

    for (int i = 0; i < count; i++)
      p->elevadores[0]->listaChamadas[i] = calls[i];
    // atenção: isso vai inserir as chamadas nas primeiras posições do vetor de
    // chamadas do elevador, não no final da fila de chamadas do elevador.
    return;
  }

  if (str[0] == 'T') {
    printf("\nINPUT: %s\n", str);

    char *newString = strtok(str, KEY);
    printf("%s\n", newString);

    int tempoNo = atoi(newString + 1);
    printf("tempo lido: %d\n", tempoNo);

    newString = strtok(NULL, KEY);
    printf("pessoa lida: %s\n", newString);
    // a pessoa eh meio inutil entao sla vey toma ai teu printf

    newString = strtok(NULL, KEY);
    bool way = (strcmp(newString, "S") == 0) ? true : false;
    printf("direcao lida: %d\n", way);

    newString = strtok(NULL, KEY);
    int origem = atoi(newString);
    printf("origem: floor %d\n", origem);

    newString = strtok(NULL, KEY);
    int destino = atoi(newString);
    printf("destino: floor %d\n", destino);

    chamadas *call = malloc(sizeof(chamadas));
    call->dir = way;
    call->tempo = tempoNo;

    call->origem = malloc(sizeof(floor));
    call->origem->id = origem;

    call->destino = malloc(sizeof(floor));
    call->destino->id = destino;

    p->elevadores[0]->listaChamadas[0] = *call;
    // atenção: esses dados vão ser inseridos na primeira posição do vetor de
    // chamadas do elevador, não no final, pois a fila de chamadas do elevador
    // não possui referência para o final.

    return;
  }
} */

int main() {
  int segundos = 0;
    struct building meuPredio;
    init(&meuPredio, 10, 1);

    floor andar1 = {1, NULL, NULL};
    floor andar2 = {2, NULL, NULL};
    floor andar3 = {3, NULL, NULL};
    floor andar4 = {4, NULL, NULL};
    floor andar5 = {5, NULL, NULL};
    floor andar6 = {6, NULL, NULL};
    
    insertOnRoute(&meuPredio.elevators[0], &andar6);
    insertOnRoute(&meuPredio.elevators[0], &andar2);
    insertOnRoute(&meuPredio.elevators[0], &andar3);

    while (1) {
        
      segundos++;
        printf("Tempo: %d seg\n\n", segundos);
      moverElevadores(&meuPredio);
      sleep(1);
        
    }

    return 0;
}
