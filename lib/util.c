#include "util.h"

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

void* trimString(String str, building* b, int noElevators) {
    str = overwriteComma(str);

    if(str[0] == "A") {
        char *newString = strtok(str, KEY);
        newString = strtok(NULL, KEY);

        int no_floors = atoi(newString);

        initialize(b, noElevators, no_floors);
        return;
    }

    if(str[0] == "E") {
        char *newString = strtok(str, KEY);

        int noElevator = atoi(newString);
        noElevator -= 1;

        if (noElevator < 0 || noElevator+1 > b->noElevators) {
            printf("numero invalido\n");
            return;
        }

        newString = strtok(NULL, KEY);
        int andar_inicio = atoi(newString);
    }
}