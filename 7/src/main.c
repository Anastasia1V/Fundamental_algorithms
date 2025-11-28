#include "mathematics.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Использование: %s <input> <output>\n", argv[0]);
        return 1;
    }
    enum status st = read_file(argv[1], argv[2]);
    if (st != SUCCESS) {
        printf("Ошибка при чтении файла: %d\n", st);
        return 1;
    }
    return 0;
}
