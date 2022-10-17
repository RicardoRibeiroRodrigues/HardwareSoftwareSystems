#include "vec_int.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _vec_int {
    int *data;
    int size;
    int capacity;
} vec_int;

vec_int *vec_int_create() {
    vec_int *new_vec = malloc(sizeof(vec_int));
    new_vec->capacity = 2;
    new_vec->data = malloc(sizeof(int) * new_vec->capacity);
    new_vec->size = 0;
    return new_vec;
}

void vec_int_destroy(vec_int **_v) {
    free((*_v)->data);
    free(*_v);
    *_v = NULL;
}

int vec_int_size(vec_int *v) {
    return v->size;
}

int vec_int_at(vec_int *v, int pos, int *val) {
    if (pos < vec_int_size(v)) {
        *val = v->data[pos];
        return 1;
    }
    return 0;
}

int vec_int_insert(vec_int *v, int pos, int val) {
    if (v->size + 1 > v->capacity) {
        v->capacity = v->capacity * 2;
        v->data = realloc(v->data, v->capacity * sizeof(int));
        if (v->data == NULL)
            return 0;
    }

    if ((pos <= vec_int_size(v)) && (pos >= 0)) {
        v->size++;
        int temp = v->data[pos];
        for (int i = v->size - 1; i > pos; i--) {
            v->data[i] = v->data[i - 1];
        }
        v->data[pos] = val;

        return 1;
    }

    return 0;
}

int vec_int_remove(vec_int *v, int pos) {
    if (v->size - 1 < (v->capacity / 4)) {
        v->capacity = v->capacity / 2;
        v->data = realloc(v->data, v->capacity * sizeof(int));
        if (v->data == NULL)
            return 0;
    }
    if ((pos < vec_int_size(v)) && (pos >= 0)) {
        for (int i = pos; i < vec_int_size(v); i++) {
            v->data[i] = v->data[i + 1];
        }
        v->size--;
        return 1;
    }

    return 0;
}
