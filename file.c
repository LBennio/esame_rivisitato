#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "data.h"
#include "checks.h"

FILE *open_file(char *name, char *mode) {
    FILE *file = NULL;
    file = fopen(name, mode);

    printf("");

    if(file == NULL) {
        printf("error");
        return NULL;
    }

    return file;
}

t_stackPtr read_file(FILE *file) {

    t_stackPtr stack = create_stack();
    t_dataPtr tempData = create_data();

    char string_read[MAX_RECORD_SIZE+1] = "\0";
    char string_temp[MAX_RECORD_SIZE+1] = "\0";
    int i = 1;

    while(fgets(string_read, MAX_RECORD_SIZE, file)) {

        string_read[strcspn(string_read, "\n")] = '\0';
        strcpy(string_temp, string_read);

        if (!check_hashtag(string_temp) && !is_valid(string_temp)) {
            printf("stringa invalida al record %d\n", i);
            return NULL;
        }

        if (check_record(string_temp, 1, i)) {
            tempData = tokenize(string_read, i);
            push(stack, tempData);

        } else {
            printf("Formato record %d non valido\n\n", i);
        }

        i++;
    }

    if(get_size(stack) < MIN_N_RECORDS) {
        printf("Numero di record insufficienti, devono essere almeno %d.\nLa lettura e' stata annullata\n\n", MIN_N_RECORDS);
        destroy_if_defined(&stack);
    }

    return stack;
}

t_dataPtr tokenize(char *input, int idx) {
    char *token = NULL;
    char *string = NULL;
    int number = 0;

    // Crea la struttura dati
    t_dataPtr data = create_data();
    if (data == NULL) {
        printf("Errore di allocazione memoria per i dati.\n");
        return NULL;
    }

    // Ottieni il primo token (la parte prima del separatore #)
    token = strtok(input, NUMBER_SEPARATOR);
    if (token == NULL) {
        printf("Errore: formato non valido al record %d, manca il separatore '#'.\n", idx);
        return NULL;
    }

    // Connetti la stringa ignorando i trattini
    string = connect_string(token, idx);
    if (string == NULL) {
        return NULL;  // Restituisci NULL se connect_string fallisce
    }

    // Ottieni il secondo token (la parte numerica dopo il #)
    token = strtok(NULL, NUMBER_SEPARATOR);
    if (token == NULL || !contains_only_numbers(token)) {
        printf("Errore: numero non conforme al record %d.\n", idx);
        free(string);  // Libera la memoria
        return NULL;
    }

    number = atoi(token);  // Converte il token in numero

    // Imposta i dati nella struttura
    set_string(data, string);
    set_number(data, number);

    free(string);  // Libera la memoria della stringa

    return data;
}

char *connect_string(char *token, int idx) {
    if (token == NULL) {
        printf("Errore: input mancante al record %d\n", idx);
        return NULL;
    }

    // Alloca memoria per la nuova stringa
    char *string = (char *)malloc((strlen(token) + 1) * sizeof(char));  // +1 per il terminatore nullo
    if (string == NULL) {
        printf("Errore di allocazione memoria al record %d.\n", idx);
        return NULL;
    }

    int j = 0;
    // Copia i caratteri ignorando i trattini
    for (int i = 0; i < strlen(token); i++) {
        if (token[i] != '-') {
            string[j++] = token[i];
        }
    }

    string[j] = '\0';  // Aggiungi il terminatore di stringa

    return string;
}

void close_file(FILE *file) {
    if(assert_file_opened(file, "Impossibile chiudere un file non aperto.")) {
        return;
    }

    fclose(file);
}

void write_stack_on_file(FILE *file, t_stackPtr stack) {
    if(!assert_file_opened(file, "file non esistente, fornire il file su cui si desidera scrivere")) {
        return;
    }

    print_stack(file, stack);
}

void print_stack_from_file(FILE *file) {
    t_dataPtr tempData = create_data();

    if(!assert_file_opened(file, "file non trovato")) {
        return;
    }

    char line[MAX_RECORD_SIZE + 1] = "\0";

    while(fgets(line, MAX_RECORD_SIZE, file)) {
        tempData = tokenize(line, 0);

        print_data(stdout, tempData);
    }
}
