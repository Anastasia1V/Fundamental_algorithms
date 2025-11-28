#include "mathematics.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

void variables_list(Variables *vars) {
    if (!vars) {
        return;
    }
    for (int i = 0; i < 26; i++) {
        if (vars->initialized[i]) {
            printf("%c = %d\n", 'A' + i, vars->values[i]);
        }
    }
}

void delete_variables(Variables *vars) {
    if (!vars) {
        return;
    }
    for (int i = 0; i < 26; i++) {
        vars->initialized[i] = 0;
        vars->values[i] = 0;
    }
}

int is_correct_name(char c) {
    return c >= 'A' && c <= 'Z';
}

int fast_pow(int base, int power, enum status *st) {
    if (power < 0) {
        if (st) {
            *st = MATH_ERROR;
        }
        return 0;
    }
    long long ans = 1;
    while (power) {
        if (power % 2) {
            ans *= base;
            if (ans > INT_MAX || ans < INT_MIN) {
                if (st) {
                    *st = MATH_ERROR;
                }
                return 0;
            }
        }
        power = power / 2;
        if (power) {
            base *= base;
            if (base > INT_MAX || base < INT_MIN) {
                if (st) {
                    *st = MATH_ERROR;
                }
                return 0;
            }
        }
    }
    if (st) {
        *st = SUCCESS;
    }
    return (int)ans;
}

static void spaces(char *s) {
    if (!s) {
        return;
    }
    char *start = s;
    while (*start && isspace((unsigned char)*start)) {
        start += 1;
    }
    char *end = s + strlen(s);
    while (end > start) {
        end -= 1;
        if (!isspace((unsigned char)*end)) {
            end += 1;
            break;
        }
    }
    if (end < start) {
        s[0] = '\0';
        return;
    }
    size_t length = end - start;
    memmove(s, start, length);
    s[length] = '\0';
}

enum status solve(const char *str, const Variables *vars, int *ans) {
    if (!str || !ans || !vars) {
        return INVALID_ARGS;
    }
    char str_copy[128];
    strncpy(str_copy, str, sizeof(str_copy) - 1);
    str_copy[sizeof(str_copy) - 1] = '\0';
    spaces(str_copy);
    char *caret = strchr(str_copy, '^');
    if (caret) {
        *caret = '\0';
        char *base_str = str_copy;
        char *exp_str = caret + 1;
        spaces(base_str);
        spaces(exp_str);
        int base, power;
        if (strlen(base_str) == 1 && is_correct_name(base_str[0])) {
            int index = base_str[0] - 'A';
            if (!vars->initialized[index]) {
                return NOT_INITIALIZED;
            }
            base = vars->values[index];
        } else if (sscanf(base_str, "%d", &base) != 1) {
            return SYNTAX_ERROR;
        }
        if (strlen(exp_str) == 1 && is_correct_name(exp_str[0])) {
            int index = exp_str[0] - 'A';
            if (!vars->initialized[index]) {
                return NOT_INITIALIZED;
            }
            power = vars->values[index];
        } else if (sscanf(exp_str, "%d", &power) != 1) {
            return SYNTAX_ERROR;
        }
        enum status st;
        *ans = fast_pow(base, power, &st);
        return st;
    }
    if (strlen(str_copy) == 1 && is_correct_name(str_copy[0])) {
        int index = str_copy[0] - 'A';
        if (!vars->initialized[index]) {
            return NOT_INITIALIZED;
        }
        *ans = vars->values[index];
        return SUCCESS;
    }
    int value;
    if (sscanf(str_copy, "%d", &value) == 1) {
        *ans = value;
        return SUCCESS;
    }
    return SYNTAX_ERROR;
}

enum status take_command(const char *command, Variables *vars, FILE *file, size_t n) {
    if (!command || !vars) {
        return INVALID_ARGS;
    }
    char str_copy[128];
    strncpy(str_copy, command, sizeof(str_copy) - 1);
    str_copy[sizeof(str_copy) - 1] = '\0';
    spaces(str_copy);
    char *equal = strchr(str_copy, '=');
    if (!equal) {
        return SYNTAX_ERROR;
    }
    *equal = '\0';
    char *var_str = str_copy;
    char *expr_str = equal + 1;
    spaces(var_str);
    spaces(expr_str);
    if (strlen(var_str) != 1 || !is_correct_name(var_str[0])) {
        return SYNTAX_ERROR;
    }
    int value;
    enum status st = solve(expr_str, vars, &value);
    if (st != SUCCESS) {
        return st;
    }
    int index = var_str[0] - 'A';
    vars->values[index] = value;
    vars->initialized[index] = 1;
    if (file) {
        fprintf(file, "Line %zu: %s -> %d\n", n, command, value);
    }
    return SUCCESS;
}

enum status read_file(const char *input, const char *path) {
    if (!input) {
        return INVALID_ARGS;
    }
    FILE *in = fopen(input, "r");
    if (!in) {
        return FILE_ERROR;
    }
    FILE *file = NULL;
    if (path) {
        file = fopen(path, "w");
        if (!file) {
            fclose(in);
            return FILE_ERROR;
        }
    }
    Variables vars = {0};
    char line[256];
    size_t n = 1;
    while (fgets(line, sizeof(line), in)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        enum status st = take_command(line, &vars, file, n);
        if (st != SUCCESS && file) {
            fprintf(file, "Ошибка на линии %zu: %d\n", n, st);
        }
        n += 1;
    }
    delete_variables(&vars);
    fclose(in);
    if (file) {
        fclose(file);
    }
    return SUCCESS;
}
