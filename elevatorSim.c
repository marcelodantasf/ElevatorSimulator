#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef char *String;
typedef struct floor floor;
typedef struct elevator elevator;
typedef struct chamadas chamadas;
typedef struct building building;

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

void mostrarElevadores(building predio) {
    for (int i = 0; i < predio.num_elevators; i++) {
        elevator elev = predio.elevators[i];
        printf("[elevador %d]\n", i + 1);
        printf("Direção atual: %s\n", elev.direction ? "subindo" : "descendo");
        printf("Andar atual: %d\n\n", elev.current_floor->floor_number);
    }
}

void subir(elevator *elevador) {
  while (elevador->current_floor && 
         elevador->current_floor->floor_number < elevador->rotaSubindo->floor_number) {
    if (elevador->current_floor->floor_number == 0) {
      printf("[Térreo]\nElevador: Subindo...\n\n");
    } else {
      printf("[%d]\nElevador: Subindo...\n\n", elevador->current_floor->floor_number);
    }

    elevador->current_floor = elevador->current_floor->next;
  }

  printf("\nChegou no andar %d!!!\n\n", elevador->rotaSubindo->floor_number);

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
    if (elevador->current_floor->floor_number == 0) {
      printf("[Térreo]\nElevador: Descendo...\n\n");
    } else {
      printf("[%d]\nElevador: Descendo...\n\n", elevador->current_floor->floor_number);
    }

    elevador->current_floor = elevador->current_floor->prev; 
  }

  printf("\nChegou no andar %d!!!\n\n", elevador->rotaDescendo->floor_number);

  floor *aux = elevador->rotaDescendo;
  elevador->rotaDescendo = elevador->rotaDescendo->next;
  free(aux);

  if (!elevador->rotaDescendo) {
    elevador->direction = true; 
  }
}

void move(elevator *elevador) {
  if (!elevador->rotaSubindo && !elevador->rotaDescendo) {
    printf("Elevador sem chamadas\n");
    return;
  }
  while (elevador->rotaSubindo) {
    
    subir(elevador);
  }

  while (elevador->rotaDescendo) {
    
    descer(elevador);
  }
}

void moverElevadores(building *predio) {
    for (int i = 0; i < predio->num_elevators; i++) {
        move(&predio->elevators[i]);
        mostrarElevadores(*predio);
    }
}

void init(struct building *predio, int num_floors, int num_elevators) 
{
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

void insertOnRoute(elevator *elevador, floor *andar) 
{
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

void escolherElevador(struct building *predio, int andar_destino, bool dir) {
  int *elevadoresPossiveis = malloc(sizeof(int) * predio->num_elevators);
  int *elevadoresPossiveis2 = malloc(sizeof(int) * predio->num_elevators);
  int *elevadoresPossiveis3 = malloc(sizeof(int) * predio->num_elevators);
  struct building *aux = malloc(sizeof(struct building));
  if (!aux) return;
  aux = predio;
  for(int i = 0; i< predio->num_elevators; i++) {
    if(predio->elevators[i].direction == dir) {
      elevadoresPossiveis[i] = i;
    }
  }

  int x = 0;
  if(dir) {
    for(int i = 0; i < sizeof(elevadoresPossiveis)/8; i++) {
     
        if(predio->elevators[elevadoresPossiveis[i]].current_floor < andar_destino) {
          elevadoresPossiveis2[x] = elevadoresPossiveis[i];
          x++;
        }
      
      
    }
  }
  else {
    for(int i = 0; i < sizeof(elevadoresPossiveis)/8; i++) {
      if(predio->elevators[elevadoresPossiveis[i]].current_floor > andar_destino) {
        elevadoresPossiveis2[x] = elevadoresPossiveis[i];
        x++;
      }
    }
  }

for(int i = 0; i < sizeof(elevadoresPossiveis2)/8; i++) {
  
}

 }

// void verificarChamadas(struct building *predio, int segundos) {
//   while(predio->)
// }

int main() {
 int segundos;
  struct building meuPredio;
  init(&meuPredio, 10, 2);
//inicializar coisas e preparar chamadas

  
  floor andar1 = {1, NULL, NULL};
  floor andar2 = {4, NULL, NULL};
  floor andar3 = {3, NULL, NULL};
  floor andar4 = {7, NULL, NULL};
  floor andar5 = {9, NULL, NULL};
  floor andar6 = {2, NULL, NULL};
  
  insertOnRoute(&meuPredio.elevators[1], &andar1);
  insertOnRoute(&meuPredio.elevators[1], &andar2);
  insertOnRoute(&meuPredio.elevators[1], &andar3);
  move(&meuPredio.elevators[1]);
  insertOnRoute(&meuPredio.elevators[1], &andar6);
   move(&meuPredio.elevators[1]);
   insertOnRoute(&meuPredio.elevators[1], &andar4);
   insertOnRoute(&meuPredio.elevators[1], &andar5);
  move(&meuPredio.elevators[1]);

  return 0;
}
