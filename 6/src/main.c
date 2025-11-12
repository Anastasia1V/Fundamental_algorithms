#include "brackets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char str[1024];
    while (1) {
        if (fgets(str, sizeof(str), stdin) == NULL) {
            break;
        }
        size_t length = strlen(str);
        if (length > 0 && str[length - 1] == '\n') {
            str[length - 1] = '\0';
            length -= 1;
        }
        if (length == 0) {
            break;
        }
        int ans = check_brackets(str);
        if (ans == 0) {
            printf("Да\n");
        }
        else if (ans == 1) {
            printf("Нет\n");
        }
        else {
            printf("Ошибка памяти\n");
        }
    }
    return 0;
}
