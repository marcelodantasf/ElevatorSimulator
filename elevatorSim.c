#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef char* String;
typedef struct andar andar;
typedef struct elevador elevador;
typedef struct chamadas chamadas;
typedef struct predio predio;

#define KEY "_"

typedef enum {
    SUBINDO,
    DESCENDO, 
    PARADO
} Direcao;

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
};

void mostrarElevadores(elevador *elevador, int i) {
    printf("\n| E%d: ", i + 1);
    printf("%s\n", elevador->percorrido);
}

void irParaCima(elevador *elevador, int seg) {
    if (!elevador->rotaCima) return;  

    andar *a = elevador->andar_atual;

    if (a->proximo && a->numero_andar < elevador->rotaCima->numero_andar) {
        elevador->andar_atual = a->proximo;
        elevador->andares_percorridos++;
    }

    if (elevador->andar_atual->numero_andar == elevador->rotaCima->numero_andar) {
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
    if (!elevador->rotaBaixo) return;  

    andar *a = elevador->andar_atual;

    if (a->anterior && a->numero_andar > elevador->rotaBaixo->numero_andar) {
        elevador->andar_atual = a->anterior;
        elevador->andares_percorridos++;
    }

    if (elevador->andar_atual->numero_andar == elevador->rotaBaixo->numero_andar) {
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
            predio->elevadores[i].andar_atual[j].proximo = (j < num_andares - 1) ? &predio->elevadores[i].andar_atual[j + 1] : NULL;
            predio->elevadores[i].andar_atual[j].anterior = (j > 0) ? &predio->elevadores[i].andar_atual[j - 1] : NULL;
        }

        predio->elevadores[i].andar_atual = &predio->elevadores[i].andar_atual[0];
    }
}

int escolherElevador(predio *predio, andar *novo_andar) {
    int melhorElevador = -1;
    int numAndares = predio->num_andares;

    for (int i = 0; i < predio->num_elevadores; i++) {
        elevador *e = &predio->elevadores[i];
        int distancia = abs(e->andar_atual->numero_andar - novo_andar->numero_andar);


        if (e->direcao == PARADO || 
            (e->direcao == SUBINDO && novo_andar->numero_andar > e->andar_atual->numero_andar) ||
            (e->direcao == DESCENDO && novo_andar->numero_andar < e->andar_atual->numero_andar)) {


            if (distancia < numAndares) {
                numAndares = distancia; 
                melhorElevador = i; 
            }
        }
    }

    return melhorElevador;
}

void inserirNaRota(predio *predio, andar *a) {
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
        } else if (novo_andar->numero_andar < elevador->andar_atual->numero_andar) {
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

int main() {
    struct predio meuPredio;
    inicializar(&meuPredio, 20, 3);

    andar andares[6] = {
        {1, NULL, NULL},
        {2, NULL, NULL},
        {3, NULL, NULL},
        {4, NULL, NULL},
        {5, NULL, NULL},
        {15, NULL, NULL}
    };

    inserirNaRota(&meuPredio, &andares[5]);
    inserirNaRota(&meuPredio, &andares[1]); 
    inserirNaRota(&meuPredio, &andares[2]); 
    inserirNaRota(&meuPredio, &andares[0]); 

    int segundos = 0;
    while (1) {
        system("clear");
        segundos++;
        printf("\n\nSegundo: %d\n", segundos-1);
        moverElevadores(&meuPredio, segundos);
        sleep(1);
        
    }

    return 0;
}