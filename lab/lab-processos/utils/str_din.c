#include "str_din.h"

str_din *str_din_create() {
    str_din *new_vec = malloc(sizeof(str_din));
    new_vec->capacity = 50;
    new_vec->data = malloc(sizeof(char) * new_vec->capacity);
    new_vec->size = 0;
    return new_vec;
}

void str_din_destroy(str_din **_v) {
    free((*_v)->data);
    free(*_v);
    *_v = NULL;
}

int str_din_size(str_din *v) {
    return v->size;
}

int str_din_at(str_din *v, int pos, char *val) {
    if (pos < str_din_size(v)) {
        *val = v->data[pos];
        return 1;
    }
    return 0;
}

int str_din_append(str_din *v, char *str, int size) {
    if ((v->size + size) >= v->capacity) {
        v->capacity = (v->size + size) * 2;
        v->data = realloc(v->data, sizeof(char) * v->capacity);
        if (v->data == NULL)
            return 0;
    }
    for (int i = 0; i < size; i++) {
        if (str_din_size(v) > 0) {
            v->data[v->size - 1 + i] = str[i];
        } else {
            v->data[v->size + i] = str[i];
        }
    }
    v->data[v->size + size] = '\0';
    v->size += size;
    return 1;
}