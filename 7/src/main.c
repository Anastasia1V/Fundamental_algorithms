#include "mathematics.h"
#include <stdio.h>

int main() {
    enum status st = read_file("7/input.txt", "7/output.txt");
    if (st != SUCCESS) {
        printf("Ошибка при чтении файла: %d\n", st);
        return 1;
    }
    return 0;
}
