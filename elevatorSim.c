#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef char *String;

typedef struct floor floor;
typedef struct elevator elevator;

struct building {
  elevator *elevators;
  int floors;
};

struct floor {
  int floor_number;
  floor *prev;
  floor *next;
};

struct elevator {
  floor *rota;
  floor *current_floor;
  bool direction; // true = subindo, false = descendo
};

void move(elevator *elevador) {
  if (!elevador->rota) {
    printf("Elevador sem chamadas\n");
    return;
  }
  
  while (elevador->current_floor->floor_number <= elevador->rota->floor_number) {
    printf("Subindo... para %d\n", elevador->rota->floor_number);
    elevador->current_floor = elevador->current_floor->next;
  }

  floor *aux = elevador->rota;
  elevador->rota = elevador->rota->next;
  free(aux);
}

void init(struct building *predio, int num_floors, int num_elevators) {
  predio->floors = num_floors;
  predio->elevators = malloc(num_elevators * sizeof(elevator));

  if (!predio->elevators) {
    printf("Erro ao alocar memória para elevadores\n");
    return;
  }

  for (int i = 0; i < num_elevators; i++) {
    predio->elevators[i].rota = NULL;
    predio->elevators[i].current_floor = NULL;
    predio->elevators[i].direction = true; 
  }
}

void insertOnRoute(elevator *elevador, floor *andar) {
  floor *novo = malloc(sizeof(floor));
  if (!novo) {
    printf("Erro ao alocar memória para o andar\n");
    return;
  }
  *novo = *andar;

  if (!elevador->rota) {
    elevador->rota = novo;
    printf("adicionou na rota");
    return;
  }

  floor *aux = elevador->rota;
  while (aux->next && aux->next->floor_number < andar->floor_number) {
    aux = aux->next;
  }

  novo->next = aux->next;
  novo->prev = aux;
  if (aux->next) {
    aux->next->prev = novo;
  }
  aux->next = novo;
  printf("\nadicionou na rota2\n");
}

int main() {
  struct building meuPredio;
    floor *andar1 = malloc(sizeof(floor));
    andar1->floor_number = 1;
    andar1->prev = NULL;
    andar1->next = NULL;
    floor *andar2 = malloc(sizeof(floor));
    andar2->floor_number = 1;
    andar2->next = NULL;
    andar2->prev = andar1;
  init(&meuPredio, 10, 2);

  printf("Número de andares: %d\n", meuPredio.floors);

insertOnRoute(&meuPredio.elevators[0], &andar1);
    insertOnRoute(&meuPredio.elevators[0], &andar2);
    move(&meuPredio.elevators[0]);
    
    //separar em func
  return 0;
}
