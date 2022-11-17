#ifndef __STR_DIN_H__
#define __STR_DIN__

#include <stdio.h>
#include <stdlib.h>

typedef struct _str_din {
    char *data;
    int size;
    int capacity;
} str_din;

str_din *str_din_create();
void str_din_destroy(str_din **v);

int str_din_size(str_din *v);

/* As seguinte operações devolvem
 * 1 se pos é uma posição válida e a operação foi bem sucedida
 * 0 caso contrário
 *
 * No caso de at, o valor é retornado na variável apontada por vi.
 */
int str_din_at(str_din *v, int pos, char *vi);
int str_din_append(str_din *v, char *str, int size);

#endif