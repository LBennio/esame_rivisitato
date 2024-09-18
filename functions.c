#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "functions.h"
#include "data.h"
#include "stack.h"
#include "checks.h"
#include "file.h"

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

                if(empty(stack)) {
                    printf("acquisire prima di procedere\n");
                    break;
                }

                print_stack(stdout, stack);

                char to_delete[MAX_STRING_SIZE + 1] = "\0";
                char to_insert[MAX_STRING_SIZE + 1] = "\0";

                do {
                    printf("dati da eliminare (formato esame#numero): ");
                    fscanf(stdin, "%s", to_delete);
                    to_delete[strcspn(to_delete, "\n")] = '\0';

                    printf("\ndata to insert (formato esame#numero): ");
                    fscanf(stdin, "%s", to_insert);
                    to_insert[strcspn(to_insert, "\n")] = '\0';

                } while(!check_record(to_delete, MAX_RECORD_SIZE, 0) && !check_record(to_insert, MAX_RECORD_SIZE, 0));

                FILE *sost_file = NULL;
                sost_file = open_file("Sostituisci.dat", "r+");

                sostitution(stack, to_delete, to_insert);

                if(empty(stack)) {
                    printf("stack non creato, riprovare\n");
                    break;
                }

                //write_stack_on_file(sost_file, stack_sostituzione);
                //print_stack_from_file(sost_file);

                print_stack(stdout, stack);
                fclose(sost_file);
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

void sostitution(t_stackPtr stack, char *to_delete, char *to_insert) {
    t_stackPtr tempStack = NULL;
    t_dataPtr tempData = NULL;

    int node_number = 0;

    tempData = tokenize(to_delete, 0);
    printf("%s %d", get_string(tempData), get_number(tempData));

    if((node_number = is_present(stack, tempData))) {
        printf("stringa ricercata non trovata\n");
        return;
    }

}

int is_present(t_stackPtr stack, t_dataPtr data) {
    t_stackPtr tempStack = create_stack();
    t_dataPtr tempData = NULL;

    while(!empty(stack)) {
        tempData = pop(stack);
        if(compare_data(data, tempData)) {

        }

        push(stack, tempData);
    }


    return NULL;
}

bool compare_data(data, tempData) {

}
