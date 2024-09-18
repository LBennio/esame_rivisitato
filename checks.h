#ifndef CHECKS_H_INCLUDED
#define CHECKS_H_INCLUDED

#include "data.h"

int get_input_int();
bool check_txt(char *nome);
bool assert_file_opened(FILE *file, const char *message);
bool check_hashtag(char *string);
bool is_valid(char *string);
bool check_record(char *input, int idx);
bool contains_only_numbers(char *str);

#endif // CHECKS_H_INCLUDED
