#include "numbers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void help() {
    printf("Использование: <десятичное число> <r>\n");
}

int main() {
    help();
    char str[256];
    char out[256];
    while (1) {
        if (fgets(str, sizeof(str), stdin) == NULL) {
            break;
        }
        size_t length = strlen(str);
        if (length == 0 || str[0] == '\n' || str[0] == '\0') {
            break;
        }
        unsigned int base = 0;
        unsigned long long number = 0;
        int read = sscanf(str, "%llu %u", &number, &base);
        if (read != 2) {
            fprintf(stderr, "Некорректный ввод.\n");
            help();
            continue;
        }
        if (base < 1 || base > 5) {
            fprintf(stderr, "Требуется 1 <= r <= 5.\n");
            help();
            continue;
        }
        enum status st = task(number, base, out, sizeof(out));
        if (st == SUCCESS) {
            printf("%s\n", out);
        } else if (st == MEMORY_ERROR) {
            fprintf(stderr, "Ошибка памяти для %llu\n", number);
        } else {
            fprintf(stderr, "Ошибка при обработке %llu\n", number);
        }
    }
    return 0;
}
