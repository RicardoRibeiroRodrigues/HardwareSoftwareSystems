/* POSIX headers */
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// Curl header.
#include <curl/curl.h>
// User defined
#include "utils/utils.h"

#define DEBUG 1

typedef struct {
    char *path;
    int fd;
} res_args;

int stats_global = 0;
void sigint_handler(int num) {
    printf("Você deseja sair mesmo ? [s/n]\n");
    char res;
    scanf("%c", &res);
    if (res == 's') {
        printf("Saindo...\n");
        stats_global = 1;
    }
}

size_t write_data(void *ptr, size_t size, size_t nmemb, res_args *res_data) {
    size_t written = write(res_data->fd, ptr, nmemb);
    if (stats_global == 1) {
        printf("Terminando o processo e deletando arquivos nao terminados\n");
        remove(res_data->path);
        exit(0);
    }
    return written;
}

void download_file(const char *url, const char *path, CURL *curl) {
    int fd = open(path, O_WRONLY | O_CREAT, 0754);
    if (fd == -1) {
        printf("Problema ao abrir o arquivo\n");
    }
    // FILE *fp = fopen(path, "wb");
    res_args res_args_download;
    res_args_download.fd = fd;
    res_args_download.path = (char *)path;

    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res_args_download);
    res = curl_easy_perform(curl);

    // Fecha o arquivo.
    close(fd);
    // fclose(fp);

    if (res != CURLE_OK) {
        fprintf(stderr, "[-] Could Not Fetch Webpage\n[+] Error : %s\n", curl_easy_strerror(res));
        printf("{%s} não pode ser baixada.\n", url);
        // Deleta o arquivo que tinha sido aberto anteriormente.
        remove(path);
        // Libera a string do path.
        free((char *)path);
        exit(-2);
    }

    // Libera a string do caminho para o arquivo da memória
    free((char *)path);
    // Ver com o maciel esse aqui.
    curl_easy_cleanup(curl);

    // printf("{%s} baixada com sucesso!\n", url);
    exit(0);
}

char **read_file_list(char *path, int *num_of_links) {
    int fd1 = open(path, O_RDONLY);
    if (((fd1 == -1) && (EACCES == errno))) {
        printf("Sem permissão para abrir os arquivos\n");
        exit(1);
    }
    str_din *str_dinamica = str_din_create();
    if (str_dinamica == NULL) {
        printf("A string nao pode ser alocada\n");
        exit(1);
    }
    char buf[1000];
    printf("Iniciando a copia de arquivos!\n");
    while (1) {
        ssize_t ret = read(fd1, buf, 1000);
        if (ret == 0)
            break;
        str_din_append(str_dinamica, buf, ret);
    }
    close(fd1);
    char **links = string_to_string_vec(str_dinamica, num_of_links);
    str_din_destroy(&str_dinamica);
    return links;
}

int main(int argc, char const *argv[]) {

    struct sigaction s_i;
    s_i.sa_handler = sigint_handler;
    sigemptyset(&s_i.sa_mask);
    s_i.sa_flags = 0;
    sigaction(SIGINT, &s_i, NULL);

    int n_processes;
    // Flag para indicar se o usuario passou apenas o link como argumento, ao invés de um arquivo com -f.
    char only_link = 0;
    char **links;

    // Se o usuario passou apenas o link como argumento, ao invés de um arquivo com -f,
    // o parse args vai retornar esse link, caso contrário, retorna o caminho para o arquivo com os links.
    char *ret = parse_args(argc, argv, &n_processes, &only_link);
    int num_of_links = 1;
    if (only_link) {
        links = malloc(sizeof(char *));
        links[0] = ret;
        num_of_links = 1;
    } else {
        links = read_file_list(ret, &num_of_links);
    }

#ifdef DEBUG
    if (!only_link) {
        printf("Number of links: %d\n", num_of_links);
        for (int i = 0; i < num_of_links; i++) {
            printf("%s\n", links[i]);
        }
    }
#endif

    CURL *curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "[-] Failed Initializing Curl\n");
        exit(-1);
    }

#ifdef DEBUG
    printf("Numero de processos passados por arg: %d\n", n_processes);
#endif

    printf("------------------------------\n");
    printf("Iniciando os downloads!\n");
    printf("------------------------------\n");

    pid_t child_processes[num_of_links];

    pid_t child = 1;
    int link_index = -1;
    int n_downloaded = 0;
    for (int i = 0; i < min(n_processes, num_of_links); i++) {
        if (child != 0) {
            child = fork();
            link_index++;
            // Processo pai guarda o pid_t de todos os seus filhos.
            child_processes[link_index] = child;
        }
        if (child == 0) {
            download_file(links[link_index], build_out_path(links[link_index]), curl);
        }
    }

    int status;
    while (n_downloaded < num_of_links) {
        pid_t finished_child = waitpid(child_processes[n_downloaded], &status, 0);
        if (finished_child == child_processes[n_downloaded]) {
            printf("{%s} baixada com sucesso!\n", links[n_downloaded]);
        }
        n_downloaded++;
        link_index++;
        // Se ainda existirem links para serem baixados, cria um processo filho para baixar o proximo link.
        // O wait garante que o número de processos não utrapasse N.
        if (link_index < num_of_links) {
            child = fork();
            if (child == 0) {
                download_file(links[link_index], build_out_path(links[link_index]), curl);
            }
        }
    }

    // Libera os links da memoria.
    if (!only_link) {
        for (int i = 0; i < num_of_links; i++) {
            free(links[i]);
        }
    }

    free(links);
    curl_easy_cleanup(curl);
    return 0;
}