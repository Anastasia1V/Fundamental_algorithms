#include "liver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help(void) {
    printf("Введите номер команды:\n");
    printf("0. Выход\n");
    printf("1. Поиск жителя\n");
    printf("2. Изменение жителя\n");
    printf("3. Удаление жителя\n");
    printf("4. Добавление жителя\n");
    printf("5. Выгрузка данных в файл\n");
    printf("6. Отмена последних N/2 модификаций\n");
}

int main(void) {
    Liver livers[256];
    size_t count = 0;
    enum status st = read_liver("3/liver.txt", livers, 256, &count);
    if (st != SUCCESS) {
        fprintf(stderr, "Ошибка при чтении файла: %d\n", st);
        return 1;
    }
    LinkedList *list = make_linked_list(livers, count);
    if (list == NULL) {
        fprintf(stderr, "Ошибка создания списка.\n");
        return 1;
    }
    help();
     while (1) {
        printf("\n");
        printf("Введите команду: ");
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }
        int v = atoi(input);
        if (v == 0) {
            printf("Выход.\n");
            break;
        } else if (v == 1) {
            find_liver(list);
        } else if (v == 2) {
            edit_liver(list);
        } else if (v == 3) {
            delete_liver(list);
        } else if (v == 4) {
            add_liver(list);
        } else if (v == 5) {
            info_to_file(list);
        } else {
            printf("Некорректная команда.\n");
        }
    }
    delete_list(list);
    return 0;
}
