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

void receive(char str[], building *p) {
    str = overwriteComma(str);

    if (str[0] == 'A') {  // Adicionar andar
        char *newString = strtok(str, KEY);
        newString = strtok(NULL, KEY);
        printf("string after decode: %s\n", newString);

        int floor_number = atoi(newString);
        printf("floor level read: %d\n", floor_number);

        // Cria um novo floor
        floor *novo_andar = malloc(sizeof(floor));
        novo_andar->floor_number = floor_number;
        novo_andar->next = NULL;
        novo_andar->prev = NULL;

        // Adiciona esse novo andar na rota do elevador 0 (ou pode-se modificar para ser dinâmico)
        insertOnRoute(&p->elevators[0], novo_andar);

        return;
    }

    if (str[0] == 'E') {  // Atualizar status do elevador
        printf("INPUT: %s\n", str);

        char *newString = strtok(str, KEY);
        printf("%s\n", newString);

        int elevatorNo = atoi(newString + 1);  // Lê o número do elevador
        printf("elevator read: %d\n", elevatorNo);

        if (elevatorNo < 0 || elevatorNo >= p->num_elevators) {
            printf("Número de elevador inválido\n");
            return;
        }

        elevator *current_elevator = &p->elevators[elevatorNo];  // Seleciona o elevador correto

        newString = strtok(NULL, KEY);
        int currentFloor = atoi(newString);  // Atualiza o andar atual do elevador
        printf("current floor read: %d\n", currentFloor);

        current_elevator->current_floor = &current_elevator->floors[currentFloor];

        newString = strtok(NULL, KEY);
        bool direction = (strcmp(newString, "S") == 0) ? true : false;  // Lê a direção (S = subindo, D = descendo)
        printf("direction read: %d\n", direction);

        current_elevator->direction = direction;

        newString = strtok(NULL, KEY);
        printf("%s\n", newString);

        int floors[10];  // Um buffer para os andares
        int count = 0;

        // Processa a lista de andares
        while (newString != NULL) {
            floors[count++] = atoi(newString);
            newString = strtok(NULL, KEY);
        }

        printf("Lista de andares:\n");
        for (int i = 0; i < count; i++) {
            printf("[%d]\n", floors[i]);

            // Cria o floor de destino
            floor *destino = malloc(sizeof(floor));
            destino->floor_number = floors[i];
            destino->next = NULL;
            destino->prev = NULL;

            // Insere na rota do elevador
            insertOnRoute(current_elevator, destino);
        }

        return;
    }

    if (str[0] == 'T') {  // Adicionar chamada
        printf("\nINPUT: %s\n", str);

        char *newString = strtok(str, KEY);
        int tempoNo = atoi(newString + 1);  // Lê o tempo
        printf("tempo lido: %d\n", tempoNo);

        newString = strtok(NULL, KEY);
        printf("pessoa lida: %s\n", newString);  // A pessoa não está sendo usada aqui

        newString = strtok(NULL, KEY);
        bool direction = (strcmp(newString, "S") == 0) ? true : false;  // Lê a direção
        printf("direcao lida: %d\n", direction);

        newString = strtok(NULL, KEY);
        int origem = atoi(newString);  // Lê o andar de origem
        printf("origem: floor %d\n", origem);

        newString = strtok(NULL, KEY);
        int destino = atoi(newString);  // Lê o andar de destino
        printf("destino: floor %d\n", destino);

        // Cria uma nova chamada
        chamadas *nova_chamada = malloc(sizeof(chamadas));
        nova_chamada->dir = direction;
        nova_chamada->tempo = tempoNo;

        // Cria o floor de origem e destino
        nova_chamada->origem = malloc(sizeof(floor));
        nova_chamada->origem->floor_number = origem;

        nova_chamada->destino = malloc(sizeof(floor));
        nova_chamada->destino->floor_number = destino;

        nova_chamada->prox = NULL;

        // Insere a nova chamada na lista de chamadas do prédio
        if (p->calls == NULL) {
            p->calls = nova_chamada;
        } else {
            chamadas *aux = p->calls;
            while (aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = nova_chamada;
        }

        return;
    }

    printf("Formato invalido, digite as instrucoes conforme as regras\n");
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
