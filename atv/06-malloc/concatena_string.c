#include <stdio.h>
#include <stdlib.h>
#include "macros_correcoes.h"

/* Recebe duas strings e concatena em uma terceira */
char *mystrcat(char *str1, char *str2) {
    int tamanho_1 = 0, tamanho_2 = 0, i;
    while (str1[tamanho_1] != '\0') 
        tamanho_1++;

    while (str2[tamanho_2] != '\0')
        tamanho_2++;

    char *str_copy = (char *) malloc((tamanho_1 + tamanho_2) * sizeof(char) + 1);
    
    if (str_copy == NULL) {
        perror("malloc");
        exit(0);
    } 

    for (i = 0; i < tamanho_1; i++) 
        str_copy[i] = str1[i];

    for (i = 0; i < tamanho_2; i++)
        str_copy[i + tamanho_1] = str2[i];
    
    str_copy[tamanho_1 + tamanho_2] = '\0';

    return str_copy;
}

int main(int argc, char *argv[]) {
    # define teste1 "str1"
    # define teste2 "str2"
    # define teste3 "0 str 3"


    char *res = mystrcat(teste1, teste2);
    assertEquals("teste 1", strcmp(res, teste1 teste2), 0);
    free(res);

    res = mystrcat(teste1, teste1);
    assertEquals("teste 2", strcmp(res, teste1 teste1), 0);
    free(res);

    res = mystrcat(teste1, teste3);
    assertEquals("teste 3", strcmp(res, teste1 teste3), 0);
    free(res);

    res = mystrcat(teste3, teste1);
    assertEquals("teste 4", strcmp(res, teste3 teste1), 0);
    free(res);

    res = mystrcat("", teste2);
    assertEquals("teste vazio 1", strcmp(res, teste2), 0);
    free(res);

    res = mystrcat(teste1, "");
    assertEquals("teste vazio 2", strcmp(res, teste1), 0);
    free(res);

    res = mystrcat("", "");
    assertEquals("teste vazio 3", strcmp(res, ""), 0);
    free(res);

    printSummary

    return 0;
}
