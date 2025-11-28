#ifndef MATHEMATICS_H
#define MATHEMATICS_H

#include <stddef.h>
#include <stdio.h>

enum status {
    SUCCESS,
    INVALID_ARGS,
    FILE_ERROR,
    SYNTAX_ERROR,
    DIVIDE_BY_ZERO,
    NOT_INITIALIZED,
    MATH_ERROR,
    MEMORY_ERROR
};

typedef struct Variables {
    int values[26];
    int initialized[26];
} Variables;

void variables_list(Variables *vars);
void delete_variables(Variables *vars);
enum status read_file(const char *input, const char *output);
enum status take_command(const char *command, Variables *vars, FILE *file, size_t *index, size_t n);
enum status solve(const char *str, const Variables *vars, int *ans);
int is_correct_name(char c);
int fast_pow(int base, int power, enum status *st);

#endif
