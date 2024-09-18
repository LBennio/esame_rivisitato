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

        if (check_record(string_temp, i)) {
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

    t_dataPtr data = create_data();

    token = strtok(input, NUMBER_SEPARATOR);
    string = connect_string(token, idx);

    token = strtok(NULL, NUMBER_SEPARATOR);
    if(!contains_only_numbers(token)) {
        printf("numero non conforme at record %d\n", idx);
    }
    number = atoi(token);

    set_string(data, string);
    set_number(data, number);

    free(string);

    return data;
}

char *connect_string(char *token, int idx) {
    char *string = (char *)malloc((strlen(token) + 1) * sizeof(char));  // +1 for the null terminator
    int j = 0;

    if (token == NULL) {
        printf("Input mancante al record %d\n", idx);
        return NULL;
    }

    // Loop through the token and copy characters to string, ignoring RECORD_SEPARATOR
    for (int i = 0; i < strlen(token); i++) {
        if (token[i] != '-') {
            string[j] = token[i];
            j++;
        }
    }

    string[j] = '\0';  // Null-terminate the string

    return string;  // Return the processed string
}

void close_file(FILE *file) {
    if(assert_file_opened(file, "Impossibile chiudere un file non aperto.")) {
        return;
    }

    fclose(file);
}
