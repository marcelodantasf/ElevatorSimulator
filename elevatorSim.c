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

struct elevator {
    floor *rotaSubindo;
    floor *rotaDescendo;
    floor *current_floor;
    floor *floors;
    bool direction; // true = subindo, false = descendo
};

struct building {
    elevator *elevators;
    int num_elevators;
    chamadas *calls;
};

void mostrarElevadores(elevator *elevador, int i) {
    printf("------------------\n");
    printf("[E%d]\n", i + 1);
    printf("Direção atual: %s\n", elevador->direction ? "subindo" : "descendo");

    if (elevador->current_floor->floor_number == 0) {
        printf("Andar atual: térreo\n\n");
    } else {
        printf("Andar atual: %d\n\n", elevador->current_floor->floor_number);
    }
}

void subir(elevator *elevador) {
    if (!elevador->rotaSubindo) return;  

    floor *and = elevador->current_floor;

    if (and->next && and->floor_number < elevador->rotaSubindo->floor_number) {
        elevador->current_floor = and->next;
    }

    if (elevador->current_floor->floor_number == elevador->rotaSubindo->floor_number) {
        floor *aux = elevador->rotaSubindo;
        elevador->rotaSubindo = elevador->rotaSubindo->next;
        free(aux);
    }

    if (!elevador->rotaSubindo) {
        elevador->direction = false;  
    }
}

void descer(elevator *elevador) {
    if (!elevador->rotaDescendo) return;  

    floor *and = elevador->current_floor;

    // Verifique se o andar atual é maior que o andar de destino
    if (and->prev && and->floor_number > elevador->rotaDescendo->floor_number) {
        elevador->current_floor = and->prev;
    }

    if (elevador->current_floor->floor_number == elevador->rotaDescendo->floor_number) {
       
        floor *aux = elevador->rotaDescendo;
        elevador->rotaDescendo = elevador->rotaDescendo->next;
        free(aux);
    }

    if (!elevador->rotaDescendo) {
        elevador->direction = true;  
    }
}

void standBy(elevator *elevador) {
    while (elevador->current_floor && elevador->current_floor != &elevador->floors[0])
        elevador->current_floor = elevador->current_floor->prev;

    elevador->direction = false;
    printf("Elevador entrou em modo stand-by\n");
}

void move(elevator *elevador) {
    if (elevador->direction && elevador->rotaSubindo) {
        subir(elevador);
    } else if (!elevador->direction && elevador->rotaDescendo) {
        descer(elevador);
    } else if(!elevador->rotaSubindo && !elevador->rotaDescendo) {
        standBy(elevador);
    }
}

void moverElevadores(building *predio) {
    for (int i = 0; i < predio->num_elevators; i++) {
        mostrarElevadores(&predio->elevators[i], i);
        move(&predio->elevators[i]);
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
    novo->next = NULL;

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
