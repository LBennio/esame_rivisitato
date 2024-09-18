#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "stack.h"

struct s_data {
    char string[MAX_STRING_SIZE];
    int number;
};

typedef struct s_data t_s_data;

t_dataPtr create_data() {
    t_dataPtr newData = (t_dataPtr)malloc(sizeof(t_s_data));

    set_string(newData, "");
    set_number(newData, 0);

    return newData;
}

void set_string(t_dataPtr newData, char *string) {
    strcpy(newData->string, string);
}

char *get_string(t_dataPtr data) {
    return data->string;
}

void set_number(t_dataPtr data, int newV) {
    data->number = newV;
}

int get_number(t_dataPtr data) {
    return data->number;
}

void print_data(FILE *in, t_dataPtr data) {
    fprintf(in, "%s %d\n", get_string(data), get_number(data));
}
