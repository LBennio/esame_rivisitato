#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdbool.h>
#include "data.h"

typedef struct s_node *t_nodePtr;
typedef struct s_stack *t_stackPtr;

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

#endif // STACK_H_INCLUDED
