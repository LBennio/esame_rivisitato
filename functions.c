#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "data.h"
#include "stack.h"
#include "checks.h"
#include "file.h"

void show_menu() {
    t_stackPtr stack = NULL;
    t_stackPtr stack_sostituzione = NULL;
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
                destroy_if_defined(&stack_sostituzione);

                stack = acquisition();

                if(empty(stack)) {
                    printf("stack non creato, riprovare\n");
                    break;
                }

                print_stack(stdout, stack);
                system("pause");
                break;

            case 2:
                FILE *sost_file = open_file("Sostituisci.dat", "r+");

                destroy_if_defined(&stack_sostituzione);
                stack_sostituzione = sostitution();

                if(empty(stack)) {
                    printf("stack non creato, riprovare\n");
                    break;
                }

                write_stack_on_file(sost_file, stack_sostituzione);
                print_stack_from_file(sost_file);
                break;

            case 3: {

                break;
            }

            case 4:
                break;

            default:
                printf("Opzione non valida!\n");
                break;
        }
    } while (choice != n_opt);

    destroy_if_defined(&stack);
}

t_stackPtr acquisition() {
    FILE *filePtr = NULL;
    t_stackPtr stack = NULL;

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
    stack = read_file(filePtr);
    close_file(filePtr);

    return stack;
}
