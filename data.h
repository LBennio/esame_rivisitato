#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#define MAX_STRING_SIZE 500
#define RECORD_SEPARATOR "-"
#define NUMBER_SEPARATOR "#"

#include <stdio.h>

typedef struct s_data *t_dataPtr; // typedef puntatore a struttura dati


typedef struct s_node *t_nodePtr;
typedef struct s_stack *t_stackPtr;

// -------------------------

t_stackPtr create_stack();
t_nodePtr create_node();
void set_nextPtr(t_nodePtr node, t_nodePtr nextPtr);
t_nodePtr get_nextPtr(t_nodePtr node);

void push(t_stackPtr stack, t_dataPtr data);
t_dataPtr pop(t_stackPtr stack);
void print_stack(FILE *file, t_stackPtr stack);

void set_head(t_stackPtr stack, t_nodePtr toSet);
t_nodePtr get_head(t_stackPtr stack);

void set_size(t_stackPtr stack, int size);
int get_size(t_stackPtr stack);

void set_data(t_nodePtr node, t_dataPtr data);
t_dataPtr get_data(t_nodePtr node);

void destroy_if_defined(t_stackPtr *stack);
bool empty(t_stackPtr stack);

// ------------------definizioni FUNZIONI STACK

t_dataPtr create_data();
void set_string(t_dataPtr newData, char *string);
char *get_string(t_dataPtr data);
void set_number(t_dataPtr data, int newV);
int get_number(t_dataPtr data);
void print_data(FILE *in, t_dataPtr data);

// -----------------definizioni FUNZIONI DATI

#endif // DATA_H_INCLUDED
