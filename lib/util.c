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