#include "utils.h"

int min(int a, int b) {
    if (a > b) {
        return b;
    } else {
        return a;
    }
}

int starts_with(const char *a, const char *b) {
    return (strncmp(a, b, strlen(b)) == 0);
}

char *parse_args(int argc, char const *argv[], int *num_of_processes, char *only_link) {

    if (argc < 2) {
        printf("Menos argumentos do que o esperado\n");
        exit(1);
    }
    char *file_path = (char *)argv[1];
    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-f") == 0) && (argc > i + 1)) {
            file_path = (char *)argv[i + 1];
        }
        if ((strcmp(argv[i], "-N") == 0) && (argc > i + 1)) {
            *num_of_processes = atoi(argv[i + 1]);
        }
        if (starts_with(argv[i], "http://") || starts_with(argv[i], "https://")) {
            *only_link = 1;
            file_path = (char *)argv[i];
        }
    }
    return file_path;
}

char **string_to_string_vec(str_din *string, int *num_of_links) {
    int actual = 0;
    int previous = 0;
    int str_n = 0;
    // Desse jeito, o máximo de links que podem ser passados é 20
    char **strings = malloc(20 * sizeof(char *));

    while (string->data[actual] != '\0') {
        if (string->data[actual] == '\n') {
            int str_len = actual - previous + 1;
            strings[str_n] = malloc(str_len * sizeof(char));
            for (int i = previous; i < actual; i++) {
                strings[str_n][i - previous] = string->data[i];
            }
            // Adiciona o \0 para deixar a string valida
            strings[str_n][str_len - 1] = '\0';
            // Incrementa o numero de strings no vetor.
            str_n++;
            previous = actual + 1;
        }
        actual++;
    }
    *num_of_links = str_n;
    return strings;
}

// https://stackoverflow.com/questions/47116974/remove-a-substring-from-a-string-in-c
// Function to remove a substring from a string.
char *strremove(char *str, const char *sub) {
    char *p, *q, *r;
    if (*sub && (q = r = strstr(str, sub)) != NULL) {
        size_t len = strlen(sub);
        while ((r = strstr(p = r + len, sub)) != NULL) {
            memmove(q, p, r - p);
            q += r - p;
        }
        memmove(q, p, strlen(p) + 1);
    }
    return str;
}

char *build_out_path(char *link) {
    // Copia o link, para nao sobrescrever sobre ele
    char *file_name = malloc(200 * sizeof(char));
    strcpy(file_name, link);
    // Remove partes indesejadas do path.
    strremove(file_name, "http://");
    strremove(file_name, "www.");
    strremove(file_name, "https://");
    strremove(file_name, ".com");

    // Troca os '.' por '_'.
    int i = 0;
    int last_point = -1;
    while (file_name[i] != '\0') {
        if (file_name[i] == '.') {
            // Garante que o ultimo '.' nao vai ser substituido.
            if (last_point != -1)
                file_name[last_point] = '_';
            last_point = i;
        } else if (file_name[i] == '/') {
            file_name[i] = '_';
        }
        i++;
    }

    // Checa se o caminho tem extensão, se não tiver, adiciona .html
    char *end = strrchr(file_name, '.');
    if ((end == NULL) || (strlen(end) > 6)) {
#ifdef DEBUG
        printf("Caminho sem extensão: %s\n", file_name);
#endif
        char extention[] = ".html";
        strcat(file_name, extention);
    }

    return file_name;
}