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
        if (power % 2 != 0) {
            ans = ans * base;
            if (ans > INT_MAX || ans < INT_MIN) {
                if (st) {
                    *st = MATH_ERROR;
                }
                return 0;
            }
        }
        power = power / 2;
        if (power) {
            long long base2 = (long long)base * (long long)base;
            if (base2 > INT_MAX || base2 < INT_MIN) {
                if (st) {
                    *st = MATH_ERROR;
                }
                return 0;
            }
            base = (int)base2;
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
    char *left = s;
    while (*left && isspace((unsigned char)*left)) {
        left += 1;
    }
    char *right = s + strlen(s);
    while (right > left) {
        right -= 1;
        if (!isspace((unsigned char)*right)) {
            right += 1;
            break;
        }
    }
    if (right < left) {
        s[0] = '\0';
        return;
    }
    size_t length = right - left;
    memmove(s, left, length);
    s[length] = '\0';
}

static enum status parse(const char *str, const Variables *vars, int *ans) {
    spaces((char *)str);
    if (strlen(str) == 0) {
        return SYNTAX_ERROR;
    }
    if (strlen(str) == 1 && is_correct_name(str[0])) {
        int index = str[0] - 'A';
        if (!vars->initialized[index]) {
            return NOT_INITIALIZED;
        }
        *ans = vars->values[index];
        return SUCCESS;
    }
    if (sscanf(str, "%d", ans) == 1) {
        return SUCCESS;
    }
    return SYNTAX_ERROR;
}

static enum status do_power(const char *str, const Variables *vars, int *ans) {
    char *caret = strchr(str, '^');
    if (caret) {
        *caret = '\0';
        char *base_str = (char *)str;
        char *power_str = caret + 1;
        spaces(base_str);
        spaces(power_str);
        int base_int = 0;
        int power_int = 0;
        enum status st = parse(base_str, vars, &base_int);
        if (st != SUCCESS) {
            return st;
        }
        st = parse(power_str, vars, &power_int);
        if (st != SUCCESS) {
            return st;
        }
        *ans = fast_pow(base_int, power_int, &st);
        return st;
    } else {
        return parse(str, vars, ans);
    }
}

static enum status do_multiply_or_divide(const char *str, const Variables *vars, int *answ) {
    char *multiply_index = strchr(str, '*');
    if (multiply_index) {
        *multiply_index = '\0';
        char *left_str = (char *)str;
        char *right_str = multiply_index + 1;
        spaces(left_str);
        spaces(right_str);
        int left_val = 0;
        int right_val = 0;
        enum status st = do_power(left_str, vars, &left_val);
        if (st != SUCCESS) {
            return st;
        }
        st = do_power(right_str, vars, &right_val);
        if (st != SUCCESS) {
            return st;
        }
        long long ans = (long long)left_val * (long long)right_val;
        if (ans > INT_MAX || ans < INT_MIN) {
            return MATH_ERROR;
        }
        *answ = (int)ans;
        return SUCCESS;
    }
    char *divide_index = strchr(str, '/');
    if (divide_index) {
        *divide_index = '\0';
        char *left_str = (char *)str;
        char *right_str = divide_index + 1;
        spaces(left_str);
        spaces(right_str);
        int left_int = 0;
        int right_int = 0;
        enum status st = do_power(left_str, vars, &left_int);
        if (st != SUCCESS) {
            return st;
        }
        st = do_power(right_str, vars, &right_int);
        if (st != SUCCESS) {
            return st;
        }
        if (right_int == 0) {
            return DIVIDE_BY_ZERO;
        }
        int ans = left_int / right_int;
        *answ = ans;
        return SUCCESS;
    }
    return do_power(str, vars, answ);
}

static enum status do_plus(const char *str, const Variables *vars, int *answ) {
    char *plus_index = strchr(str, '+');
    if (plus_index) {
        *plus_index = '\0';
        char *left_str = (char *)str;
        char *right_str = plus_index + 1;
        spaces(left_str);
        spaces(right_str);
        int left_int = 0;
        int right_int = 0;
        enum status st = do_multiply_or_divide(left_str, vars, &left_int);
        if (st != SUCCESS) {
            return st;
        }
        st = do_multiply_or_divide(right_str, vars, &right_int);
        if (st != SUCCESS) {
            return st;
        }
        long long ans = (long long)left_int + (long long)right_int;
        if (ans > INT_MAX || ans < INT_MIN) {
            return MATH_ERROR;
        }
        *answ = (int)ans;
        return SUCCESS;
    }
    return do_multiply_or_divide(str, vars, answ);
}

static enum status do_minus(const char *str, const Variables *vars, int *answ) {
    char *minus_index = strchr(str, '-');
    if (minus_index) {
        *minus_index = '\0';
        char *left_str = (char *)str;
        char *right_str = minus_index + 1;
        spaces(left_str);
        spaces(right_str);
        int left_int = 0;
        int right_int = 0;
        enum status st = do_plus(left_str, vars, &left_int);
        if (st != SUCCESS) {
            return st;
        }
        st = do_plus(right_str, vars, &right_int);
        if (st != SUCCESS) {
            return st;
        }
        long long ans = (long long)left_int - (long long)right_int;
        if (ans > INT_MAX || ans < INT_MIN) {
            return MATH_ERROR;
        }
        *answ = (int)ans;
        return SUCCESS;
    }
    return do_plus(str, vars, answ);
}

enum status solve(const char *str, const Variables *vars, int *ans) {
    if (!str || !ans || !vars) {
        return INVALID_ARGS;
    }
    char copy_str[128];
    strncpy(copy_str, str, sizeof(copy_str) - 1);
    copy_str[sizeof(copy_str) - 1] = '\0';
    spaces(copy_str);
    return do_minus(copy_str, vars, ans);
}

static char *find_next_variable_start(char *s, char *end) {
    char *c = s;
    while (c < end && *c != '\0') {
        if (isupper((unsigned char)*c)) {
            char *next = c + 1;
            while (next < end && isspace((unsigned char)*next)) {
                next += 1;
            }
            if (next < end && *next == '=') {
                return c;
            }
        }
        c += 1;
    }
    return NULL;
}

enum status take_command(const char *command, Variables *vars, FILE *file, size_t n) {
    if (!command || !vars) {
        return INVALID_ARGS;
    }
    char copy_str[512];
    strncpy(copy_str, command, sizeof(copy_str) - 1);
    copy_str[sizeof(copy_str) - 1] = '\0';
    spaces(copy_str);
    char *c = copy_str;
    char *end_ptr = copy_str + strlen(copy_str);
    while (1) {
        while (*c && isspace((unsigned char)*c)) {
            c += 1;
        }
        if (*c == '\0') {
            break;
        }
        if (!isupper((unsigned char)*c)) {
            char *next_start = find_next_variable_start(c, end_ptr);
            if (next_start == NULL) {
                break;
            } else {
                c = next_start;
            }
        }
        char *equal = strchr(c, '=');
        if (!equal) {
            break;
        }
        char *variable_name_index = c;
        while (variable_name_index < equal && isspace((unsigned char)*variable_name_index)) {
            variable_name_index += 1;
        }
        if (variable_name_index >= equal) {
            return SYNTAX_ERROR;
        }
        char variable_char = *variable_name_index;
        if (!isupper((unsigned char)variable_char)) {
            return SYNTAX_ERROR;
        }
        char *start_after_equal = equal + 1;
        char *next_command = find_next_variable_start(start_after_equal, end_ptr);
        size_t str_length = 0;
        if (next_command != NULL) {
            str_length = (size_t)(next_command - start_after_equal);
        } else {
            str_length = (size_t)(end_ptr - start_after_equal);
        }
        char str[256];
        if (str_length >= sizeof(str)) {
            return SYNTAX_ERROR;
        }
        strncpy(str, start_after_equal, str_length);
        str[str_length] = '\0';
        spaces(str);
        int value = 0;
        enum status st = solve(str, vars, &value);
        if (st != SUCCESS) {
            if (file) {
                fprintf(file, "Ошибка на линии %zu: %d\n", n, st);
            } else {
                printf("Ошибка на линии %zu: %d\n", n, st);
            }
            return st;
        }
        int index = variable_char - 'A';
        vars->values[index] = value;
        vars->initialized[index] = 1;
        char line[512];
        int written = snprintf(line, sizeof(line), "%c = %s", variable_char, str);
        if (written < 0) {
            if (file) {
                fprintf(file, "Ошибка на линии %zu: %d\n", n, SYNTAX_ERROR);
            } else {
                printf("Ошибка на линии %zu: %d\n", n, SYNTAX_ERROR);
            }
            return SYNTAX_ERROR;
        }
        if (file) {
            fprintf(file, "Строка %zu: %s\n", n, line);
            fprintf(file, "Результат: %d\n\n", value);
        } else {
            printf("Строка %zu: %s\n", n, line);
            printf("Результат: %d\n\n", value);
        }
        if (next_command != NULL) {
            c = next_command;
        } else {
            break;
        }
    }
    return SUCCESS;
}

enum status read_file(const char *input, const char *output) {
    if (!input) {
        return INVALID_ARGS;
    }
    FILE *in = fopen(input, "r");
    if (!in) {
        return FILE_ERROR;
    }
    FILE *out = NULL;
    if (output) {
        out = fopen(output, "w");
        if (!out) {
            fclose(in);
            return FILE_ERROR;
        }
    }
    Variables vars = {0};
    char str[512];
    size_t n = 1;
    while (fgets(str, sizeof(str), in)) {
        size_t length = strlen(str);
        if (length > 0 && str[length - 1] == '\n') {
            str[length - 1] = '\0';
            length -= 1;
        }
        enum status st = take_command(str, &vars, out, n);
        if (st != SUCCESS) {
            if (out) {
                fprintf(out, "Ошибка на линии %zu: %d\n", n, st);
            } else {
                printf("Ошибка на линии %zu: %d\n", n, st);
            }
        }
        n += 1;
    }
    delete_variables(&vars);
    fclose(in);
    if (out) {
        fclose(out);
    }
    return SUCCESS;
}
