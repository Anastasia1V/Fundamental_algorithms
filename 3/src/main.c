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
        fprintf(stderr, "Ошибка при чтении файла.\n");
        return 1;
    }
    LinkedList *list = make_linked_list(livers, count);
    if (list == NULL) {
        fprintf(stderr, "Ошибка создания списка.\n");
        return 1;
    }
    Undo *undo = create_undo_list();
    if (undo == NULL) {
        fprintf(stderr, "Ошибка создания Undo.\n");
        delete_list(list);
        return 1;
    }
    if (add_to_undo(undo, list) != SUCCESS) {
        fprintf(stderr, "Не удалось сохранить начальное состояние для Undo.\n");
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
            enum status st = edit_liver(list);
            if (st == SUCCESS) {
                if (add_to_undo(undo, list) != SUCCESS) {
                    fprintf(stderr, "2. Не удалось сохранить состояние Undo.\n");
                }
            } else {
                printf("edit не выполнено.\n");
            }
        } else if (v == 3) {
            enum status st = delete_liver(list);
            if (st == SUCCESS) {
                if (add_to_undo(undo, list) != SUCCESS) {
                    fprintf(stderr, "3. Не удалось сохранить состояние после удаления.\n");
                }
            } else {
                printf("delete не выполнено.\n");
            }
        } else if (v == 4) {
            enum status st = add_liver(list);
            if (st == SUCCESS) {
                if (add_to_undo(undo, list) != SUCCESS) {
                    fprintf(stderr, "4. Не удалось сохранить состояние после добавления.\n");
                }
            } else {
                printf("add не выполнено.\n");
            }
        } else if (v == 5) {
            enum status st = info_to_file(list);
            if (st != SUCCESS) {
                printf("Ошибка при выгрузке данных в файл.\n");
            }
        } else if (v == 6) {
            enum status st = undo_half(undo, &list);
            if (st != SUCCESS) {
                printf("Undo не выполнено.\n");
            } else {
                printf("Undo выполнено.\n");
            }
        } else {
            printf("Некорректная команда.\n");
        }
    }
    delete_list(list);
    return 0;
}
