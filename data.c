#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "data.h"
#include "stack.h"

// -------------------------------
struct s_data {
    char string[MAX_STRING_SIZE];
    int number;
};

typedef struct s_data t_s_data;

// --------------------------DATA

struct s_node {
    t_dataPtr data;
    t_nodePtr nextPtr;
};

struct s_stack {
    t_nodePtr head;
    int size;
};

typedef struct s_node t_s_node;
typedef struct s_stack t_s_stack;

// --------------------------STACK

t_stackPtr create_stack() {
    t_stackPtr newStack = (t_stackPtr)malloc(sizeof(t_s_stack));

    if(newStack == NULL) {
        printf("memoria non disponibile per la creazione dello stack\n");
        return NULL;
    }

    set_head(newStack, NULL);
    set_size(newStack, 0);
    return newStack;
}

t_nodePtr create_node() {
    t_nodePtr newNode = (t_nodePtr)malloc(sizeof(t_s_node));
    t_dataPtr newData = create_data();

    set_data(newNode, newData);

    set_nextPtr(newNode, NULL);

    return newNode;
}

void set_nextPtr(t_nodePtr node, t_nodePtr nextPtr) {
    node->nextPtr = nextPtr;
}

t_nodePtr get_nextPtr(t_nodePtr node) {
    return node->nextPtr;
}

void push(t_stackPtr stack, t_dataPtr data) {

    t_nodePtr newNode = create_node();

    if(!newNode) {
        printf("impossibile creare nuovo elemento nella pila, ritorno\n");
        return;
    }

    if(empty(stack)) {
        set_data(newNode, data);

        set_head(stack, newNode);
        set_size(stack, get_size(stack)+1);
        return;
    }

    set_data(newNode, data);

    set_nextPtr(newNode, get_head(stack));

    set_head(stack, newNode);
    set_size(stack, get_size(stack)+1);
}

t_dataPtr pop(t_stackPtr stack) {
    if(empty(stack)) {
        printf("stack vuoto, acquisire prima di poter procedere\n");
        return NULL;
    }

    t_nodePtr removed = get_head(stack);
    t_dataPtr removedData = NULL;

    if(removed) {
        set_head(stack, get_nextPtr(removed));
        removedData = get_data(removed);

        free(removed);

        set_size(stack, get_size(stack) - 1);
    }

    return removedData;
}

void print_stack(FILE *file, t_stackPtr stack) {
    if(empty(stack)) {
        printf("stack non acquisito, acquisire da un file e riprovare\n");
        return;
    }

    t_stackPtr tempStack = create_stack();
    t_dataPtr tempData = NULL;

    while(!empty(stack)) {
        tempData = pop(stack);

        print_data(file, tempData);

        push(tempStack, tempData);
    }

    while(!empty(tempStack)) {
        tempData = pop(tempStack);
        push(stack, tempData);
    }

    destroy_if_defined(&tempStack);
}

void set_head(t_stackPtr stack, t_nodePtr toSet) {
    stack->head = toSet;
}

t_nodePtr get_head(t_stackPtr stack) {
    return stack->head;
}

void set_size(t_stackPtr stack, int size) {
    stack->size = size;
}

int get_size(t_stackPtr stack) {
    return stack->size;
}

void set_data(t_nodePtr node, t_dataPtr data) {
    node->data = data;
}

t_dataPtr get_data(t_nodePtr node) {
    return node->data;
}

void clear_stack(t_stackPtr stack) {
    while(!empty(stack)) {
        t_dataPtr data = pop(stack);
        free(data);
    }
}

void destroy_if_defined(t_stackPtr *stack) {
    if(*stack != NULL) {
        clear_stack(*stack);
        free(*stack);

        *stack = NULL;
    }
}

bool empty(t_stackPtr stack) {
    return get_head(stack) == NULL;
}

// ---------------------------FUNZIONI STACK

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

// ------------------------------FUNZIONI DATI
