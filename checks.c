#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "checks.h"

int get_input_int() {
    int n;

    n = _getche();

    return n - '0';
}

bool check_txt(char *nome) {
    int len = strlen(nome);
    return (strcmp(nome + len - 4, ".txt") != 0 || len < 4);
}

bool check_int_length(int n, int min, int max) {
    if(n < min || n > max) {
        return false;
    }

    return true;
}

bool assert_file_opened(FILE *file, const char *message) {
    if (file == NULL) {
        fprintf(stderr, "Errore: %s\n", message);
        return false;
    }

    return true;
}

bool check_hashtag(char *string) {
    if(!strstr(string, "#")) {
        return false;
    }

    return true;
}

bool is_valid(char *string) {
    int i = 0;
    while (string[i] != '\0') {

        if (i % 2 == 0) {
            if (!islower(string[i])) {
                return false;
            }

        } else {
            if (string[i] != '-') {
                return false;
            }
        }
        i++;
    }

    if (i % 2 == 0 && string[i - 1] == '-') {
        return false;
    }

    return true;
}

bool check_record(char *in, int max_length, int idx) {
    char *token = NULL;
    int i;

    char input[MAX_STRING_SIZE + 1] = "\0";

    strcpy(input, in);

    token = strtok(input, NUMBER_SEPARATOR);
    char *num = strtok(NULL, "\n");

    if(!check_int_length(max_length, 1, MAX_RECORD_SIZE)) {
        printf("lunghezza del carattere oltre i limiti permessi\n");
        return false;
    }

    if (token == NULL || num == NULL) {
        printf("'#' non trovato nel record %d\n", idx);
        return false;
    }

    if(!contains_only_numbers(num)) {
        printf("numero assente nel record %d\b", idx);
        return false;
    }

    char *c_token = strtok(token, RECORD_SEPARATOR);

    while (c_token != NULL) {

        if (strlen(c_token) > max_length || !islower(c_token[0])) {
            printf("carattere non valido al record %d\n", idx);
            return false;
        }

        c_token = strtok(NULL, RECORD_SEPARATOR);
    }

    return true;
}

bool contains_only_numbers(char *str) {
    if (str == NULL || *str == '\0')
    {
        return false;
    }
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

