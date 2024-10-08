/**
 * @file main.c
 * @brief Implementazione di un menu per la gestione di una pila di studenti con operazioni di acquisizione, sostituzione e ordinamento.
 *
 * Questo programma permette di gestire una pila di studenti leggendo i dati da file, eseguendo sostituzioni, e ordinando i dati in modo ascendente o discendente.
 *
 * @authors:
 * - Faliero Samuele Belardinelli (mat: 797163)
 * - Ennio Lo Buono (mat: 801933)
 */

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
t_dataPtr tokenize_united(char *input_string);

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

                // write_stack_on_file(sost_file, stack);
                // print_stack_from_file(sost_file);

                fclose(sost_file);
                break;

            case 3:
                system("cls");

                if(stack == NULL) {
                    printf("acquisire prima di procedere\n");
                    break;
                }

                // stampa stack per controllo
                print_stack(stdout, stack);

                // puntatore a file per il registro dello stack ordinato
                FILE *ord_file = NULL;
                ord_file = open_file("Ordina.dat", "r+");

                // funzione di ordinamento per punto 3
                order(stack);

                // write_stack_on_file(sost_file, stack);
                // print_stack_from_file(sost_file);

                fclose(ord_file);

                system("break");
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
    t_dataPtr dataToDelete = tokenize_united(to_delete);
    t_dataPtr dataToInsert = tokenize_united(to_insert);

    // Verifica se l'elemento da eliminare � presente
    if (!is_present(stack, dataToDelete)) {
        printf("Errore: elemento da eliminare non trovato.\n");
        return;
    }

    // Se l'elemento � trovato, esegui la sostituzione
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

t_dataPtr tokenize_united(char *input_string) {
    char *token = NULL;
    int number = 0;

    t_dataPtr data = create_data();

    token = strtok(input_string, NUMBER_SEPARATOR);
    set_string(data, token);

    token = strtok(NULL, NUMBER_SEPARATOR);
    if(!contains_only_numbers(token)) {
        printf("numero non conforme nel record inserito\n");
    }
    number = atoi(token);

    set_number(data, number);

    return data;
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
typedef int (*asc_desc)(t_dataPtr, t_dataPtr);

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

int ascending(t_dataPtr a, t_dataPtr b) {
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

int descending(t_dataPtr a, t_dataPtr b) {
    return -ascending(a, b);
}


// Funzione di ordinamento utilizzando l'insertion sort
void insertion_sort(t_stackPtr stack, asc_desc cmp) {
    t_stackPtr sorted = create_stack();

    while (!empty(stack)) {
        t_dataPtr current = pop(stack);

        print_data(stdout, current);

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
void order(t_stackPtr stack) {

    // controllo presenza stack
    if(stack == NULL) {
        printf("acquisire prima di proseguire\n");
        return;
    }

    // definizione variabile di tipo puntatore a file inizializzata a NULL
    asc_desc sorting = NULL;

    // "mouse" per scegliere l'opzione
    int choice;

    // array di opzioni da stampare
    char *options[] = {"Ascending", "Descending", "Return"};
    // numero opzioni nell'array
    int n_opt = sizeof(options)/sizeof(char*);

    // loop di controllo input
    do {
        // stampa opzioni
        for(int i = 1; i <= n_opt; i++) {
            printf("<%d> %s\n", i, options[i-1]);
        }

        printf("> ");
        // input numero per il mouse
        choice = get_input_int();

        switch(choice) {
            // ASCENDENT definito in functions.h come "1"
            case ASCENDENT:
                // assegnare l'indirizzo della funzione "ascending" a asc_desk (linea 255 e 339)
                sorting = &ascending;
                break;

            // DESCENDENT definito in functions.h come "2"
            case DESCENDENT:
                // assegnare l'indirizzo della funzione "descending" a asc_desk (linea 255 e 339)
                sorting = &descending;
                break;

            // RETURN definito in functions.h come "3"
            case RETURN:
                // ritorno
                return;

            default:
                printf("input invalido\n");
                break;
        }

    // se una delle opzioni è cliccata, continua
    } while(choice != n_opt && choice != ASCENDENT && choice != DESCENDENT);

    // controlla la selezione di un metodo di sorting
    if(sorting == NULL) {
        printf("nessun metodo di sorting selezionato, riprovare\n");
        return;
    }

    // esecuzione funzione puntata per ordinare stack(passato alla funzione) direttamente
    insertion_sort(stack, sorting);
    system("break");
    // scrittura su file
    //write_stack_on_file(file, stack);

    // lettura da file a console
    //print_stack_from_file(file, stack);

    print_stack(stdout, stack);
}
