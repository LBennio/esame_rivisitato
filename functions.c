#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "functions.h"
#include "data.h"
#include "stack.h"
#include "checks.h"
#include "file.h"


bool compare_data(data, tempData);
bool is_present(t_stackPtr stack, t_dataPtr data);

void show_menu() {
    t_stackPtr stack = NULL;
    int choice = 0;

    printf("---- studenti ----\n");

    char *options[] = {"Acquisisci", "Sostituisci", "Ordina", "Esci"};
    int n_opt = sizeof(options)/sizeof(char*);

    do {
        for (int i = 0; i < n_opt; i++) {
            printf("<%d> %s\n", i + 1, options[i]);
        }

        printf("> ");
        choice = get_input_int();

        switch (choice) {
            case 1:
                system("cls");

                destroy_if_defined(&stack);

                stack = acquisition();

                if(empty(stack)) {
                    printf("stack non creato, riprovare\n");
                    break;
                }

                print_stack(stdout, stack);
                system("pause");
                break;

            case 2:
                system("cls");

                if(stack == NULL) {
                    printf("acquisire prima di procedere\n");
                    break;
                }

                print_stack(stdout, stack);

                FILE *sost_file = NULL;
                sost_file = open_file("Sostituisci.dat", "r+");

                substitute(stack);

                write_stack_on_file(sost_file, stack);
                print_stack_from_file(sost_file);

                print_stack(stdout, stack);
                fclose(sost_file);
                break;

            case 3:
                system("cls");

                handle_sort(stack);

                system("pause");
                    break;

            case 4:
                break;

            default:
                printf("Opzione non valida!\n");
                break;
        }
    } while (choice != n_opt);

    destroy_if_defined(&stack);
}

t_stackPtr acquisition(t_stackPtr stack) {
    FILE *filePtr = NULL;
    t_stackPtr tempStack = NULL;

    char nome[MAX_STRING_SIZE];

	do {
        int c;

        while ((c = getchar()) != '\n' && c != EOF);

        printf("File(.txt): ");
        fscanf(stdin, "%s", nome);
        nome[strcspn(nome, "\n")] = '\0';

        if(check_txt(nome)) {
            printf("Nome del file non valido\n");
        }

    } while(check_txt(nome));

    filePtr = open_file(nome, "r");

    if(filePtr == NULL) {
        printf("yupp file");
        return NULL;
    }
    tempStack = read_file(filePtr);
    close_file(filePtr);

    return tempStack;
}

void substitute(t_stackPtr stack) {
    char to_delete[MAX_STRING_SIZE + 1] = "\0";
    char to_insert[MAX_STRING_SIZE + 1] = "\0";

    // Ciclo per assicurarsi che entrambi i record siano validi
    do {
        printf("Dati da eliminare (formato esame#numero): ");
        fscanf(stdin, "%s", to_delete);
        to_delete[strcspn(to_delete, "\n")] = '\0';

        printf("\nDati da inserire (formato esame#numero): ");
        fscanf(stdin, "%s", to_insert);
        to_insert[strcspn(to_insert, "\n")] = '\0';
    } while (!check_record(to_delete, MAX_RECORD_SIZE, 0) || !check_record(to_insert, MAX_RECORD_SIZE, 0));

    // Chiamata alla funzione di sostituzione
    sostitution(stack, to_delete, to_insert);
}

void sostitution(t_stackPtr stack, char *to_delete, char *to_insert) {
    // Tokenizza i dati da eliminare e quelli da inserire
    t_dataPtr dataToDelete = tokenize(to_delete, 0);
    t_dataPtr dataToInsert = tokenize(to_insert, 0);

    // Verifica se l'elemento da eliminare è presente
    if (!is_present(stack, dataToDelete)) {
        printf("Errore: elemento da eliminare non trovato.\n");
        return;
    }

    // Se l'elemento è trovato, esegui la sostituzione
    exchange_data(stack, dataToDelete, dataToInsert);
}

bool is_present(t_stackPtr stack, t_dataPtr data) {
    t_stackPtr tempStack = create_stack();
    t_dataPtr tempData = NULL;
    bool found = false;

    // Cerca l'elemento nel stack
    while (!empty(stack)) {
        tempData = pop(stack);
        if (compare_data(data, tempData)) {
            found = true;
        }
        push(tempStack, tempData);  // Ricostruisce lo stack temporaneo
    }

    // Ripristina lo stack originale
    while (!empty(tempStack)) {
        push(stack, pop(tempStack));
    }

    destroy_if_defined(&tempStack);  // Libera la memoria temporanea
    return found;
}

bool compare_data(t_dataPtr data1, t_dataPtr data2) {
    return (strcmp(get_string(data1), get_string(data2)) == 0 && get_number(data1) == get_number(data2));
}

void exchange_data(t_stackPtr stack, t_dataPtr toDelete, t_dataPtr toInsert) {
    t_stackPtr tempStack = create_stack();
    t_dataPtr tempData = NULL;

    // Cerca e sostituisce l'elemento
    while (!empty(stack)) {
        tempData = pop(stack);
        if (compare_data(tempData, toDelete)) {
            push(tempStack, toInsert);  // Sostituisce l'elemento
        } else {
            push(tempStack, tempData);  // Mantiene l'elemento esistente
        }
    }

    // Ripristina lo stack originale
    while (!empty(tempStack)) {
        push(stack, pop(tempStack));
    }

    destroy_if_defined(&tempStack);  // Libera la memoria temporanea
}


// Definire i puntatori a funzione per l'ordinamento
typedef int (*compare_func)(t_dataPtr, t_dataPtr);

// Funzione per calcolare il numero di caratteri speciali
int count_special_chars(const char *str) {
    int count = 0;
    while (*str) {
        if (!isalnum(*str)) {
            count++;
        }
        str++;
    }
    return count;
}

// Funzione per calcolare il numero di consonanti
int count_consonants(const char *str) {
    int count = 0;
    while (*str) {
        if (isalpha(*str) && !strchr("AEIOUaeiou", *str)) {
            count++;
        }
        str++;
    }
    return count;
}

// Funzione di confronto per l'ordinamento crescente
int compare_ascending(t_dataPtr a, t_dataPtr b) {
    int len_a = strlen(get_string(a));
    int len_b = strlen(get_string(b));

    // Prima criterio: numero di caratteri
    if (len_a != len_b) return len_a - len_b;

    // Secondo criterio: numero di caratteri speciali
    int spec_a = count_special_chars(get_string(a));
    int spec_b = count_special_chars(get_string(b));
    if (spec_a != spec_b) return spec_a - spec_b;

    // Terzo criterio: numero di consonanti
    int cons_a = count_consonants(get_string(a));
    int cons_b = count_consonants(get_string(b));
    return cons_a - cons_b;
}

// Funzione di confronto per l'ordinamento decrescente
int compare_descending(t_dataPtr a, t_dataPtr b) {
    return -compare_ascending(a, b);
}

// Funzione di ordinamento utilizzando l'insertion sort
void insertion_sort(t_stackPtr stack, compare_func cmp) {
    t_stackPtr sorted = create_stack();

    while (!empty(stack)) {
        t_dataPtr current = pop(stack);

        while (!empty(sorted) && cmp(get_head(sorted), current) > 0) {
            push(stack, pop(sorted));
        }

        push(sorted, current);
    }

    // Riempire lo stack originale ordinato
    while (!empty(sorted)) {
        push(stack, pop(sorted));
    }
}

// Funzione per gestire il case 3: ordinamento e salvataggio
void handle_sort(t_stackPtr stack) {

    if (empty(stack)) {
        printf("Acquisire prima di procedere\n");
        return;
    }

    printf("here");

    t_stackPtr stack_ascending = create_stack();
    t_stackPtr stack_descending = create_stack();
    t_nodePtr headNode = create_node();
    t_stackPtr tempStack = create_stack();

    // Clonare lo stack originale
    while (!empty(stack)) {
        headNode = pop(stack);
        push(tempStack, headNode);
        push(stack_ascending, headNode);
    }

    printf("here");

    // Ordinare in modo crescente
    insertion_sort(stack_ascending, compare_ascending);

    // Ordinare in modo decrescente (copia dallo stack temporaneo)
    while (!empty(tempStack)) {
        push(stack_descending, pop(tempStack));
    }

    insertion_sort(stack_descending, compare_descending);

    // Salvare i risultati su file ordina.dat
    FILE *file = open_file("ordina.dat", "r+");

    if (!file) {
        printf("Errore nell'apertura del file\n");
        return;
    }

    write_stack_on_file(file, stack_ascending);  // Salva stack crescente
    write_stack_on_file(file, stack_descending); // Salva stack decrescente

    printf("\n--- Stack Crescente ---\n");
    // print_stack_from_file(file); // Legge e stampa lo stack crescente
    print_stack(stdout, stack_ascending);
    printf("\n--- Stack Decrescente ---\n");
    // print_stack_from_file(file); // Legge e stampa lo stack decrescente
    print_stack(stdout, stack_descending);

    fclose(file);
}
