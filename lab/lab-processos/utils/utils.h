#ifndef UTILS_H
#define UTILS_H

#include "str_din.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int DEBUG;

int min(int a, int b);
char *parse_args(int argc, char const *argv[], int *num_of_processes, char *only_link);
char **string_to_string_vec(str_din *string, int *num_of_links);
char *strremove(char *str, const char *sub);
char *build_out_path(char *link);

#endif
