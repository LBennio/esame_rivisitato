#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "stack.h"
#include "data.h"

void show_menu();
t_stackPtr acquisition();
void sostitution(t_stackPtr stack, char *to_delete, char *to_insert);
void handle_sort(t_stackPtr stack);

#endif // FUNCTIONS_H_INCLUDED
