#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#define MAX_STRING_SIZE 500
#define RECORD_SEPARATOR "-"
#define NUMBER_SEPARATOR "#"

#include <stdio.h>

typedef struct s_data *t_dataPtr;

t_dataPtr create_data();
void set_string(t_dataPtr newData, char *string);
char *get_string(t_dataPtr data);
void set_number(t_dataPtr data, int newV);
int get_number(t_dataPtr data);
void print_data(FILE *in, t_dataPtr data);

#endif // DATA_H_INCLUDED
