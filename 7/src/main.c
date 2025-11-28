#include "mathematics.h"
#include <stdio.h>

int main() {
    Variables vars = {0};
    printf("Тест простых команд:\n");
    take_command("A = 2", &vars, NULL, 1);
    take_command("B = 3", &vars, NULL, 2);
    take_command("C = A^B", &vars, NULL, 3);
    int ans;
    if (solve("C", &vars, &ans) == SUCCESS) {
        printf("C = %d\n", ans);
    }
    if (solve("2^5", &vars, &ans) == SUCCESS) {
        printf("2^5 = %d\n", ans);
    }
    delete_variables(&vars);
    enum status st = read_file("7/input.txt", "7/output.txt");
    if (st != SUCCESS) {
        printf("Ошибка при чтении файла: %d\n", st);
        return 1;
    }
    return 0;
}
