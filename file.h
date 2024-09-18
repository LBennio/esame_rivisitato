#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <stdio.h>
#include "stack.h"
#include "data.h"

#define MAX_RECORD_SIZE 1024
#define MIN_N_RECORDS 4

FILE *open_file(char *name, char *mode);
t_stackPtr read_file(FILE *file);
t_dataPtr tokenize(char *input, int idx);
char *connect_string(char *token, int idx);
void close_file(FILE *file);

#endif // FILE_H_INCLUDED
