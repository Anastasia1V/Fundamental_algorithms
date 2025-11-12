#include "brackets.h"
#include <stdlib.h>
#include <string.h>

int check_brackets(const char *str) {
    if (str == NULL) {
        return 1;
    }
    size_t length = strlen(str);
    if (length == 0) {
        return 0;
    }
    char *brackets = malloc(length);
    if (brackets == NULL) {
        return 2;
    }
    size_t index = 0;
    for (size_t i = 0; i < length; i++) {
        char c = str[i];
        if (c == '(' || c == '[' || c == '{' || c == '<') {
            if (index >= length) {
                free(brackets);
                return 2;
            }
            brackets[index] = c;
            index += 1;
        } else if (c == ')') {
            if (index == 0) {
                free(brackets);
                return 1;
            }
            index -= 1;
            if (brackets[index] != '(') {
                free(brackets);
                return 1;
            }
        } else if (c == ']') {
            if (index == 0) {
                free(brackets);
                return 1;
            }
            index -= 1;
            if (brackets[index] != '[') {
                free(brackets);
                return 1;
            }
        } else if (c == '}') {
            if (index == 0) {
                free(brackets);
                return 1;
            }
            index -= 1;
            if (brackets[index] != '{') {
                free(brackets);
                return 1;
            }
        } else if (c == '>') {
            if (index == 0) {
                free(brackets);
                return 1;
            }
            index -= 1;
            if (brackets[index] != '<') {
                free(brackets);
                return 1;
            }
        }
    }
    free(brackets);
    if (index == 0) {
        return 0;
    }
    return 1;
}
