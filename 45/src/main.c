#include "mail.h"
#include <stdio.h>

int main(void) {
    printf("create_system\n");
    MailSystem *sys = create_system("log.txt");
    if (!sys) {
        printf("Ошибка при создании системы\n");
        return 1;
    } else {
        printf("Система успешно создана\n");
    }
    printf("\n");
    printf("add_office\n");
    unsigned int neighbors1[] = {2, 3};
    if (add_office(sys, 1, 10, neighbors1, 2) == SUCCESS) {
        printf("Офис 1 добавлен\n");
    }
    unsigned int neighbors2[] = {1};
    if (add_office(sys, 2, 5, neighbors2, 1) == SUCCESS) {
        printf("Офис 2 добавлен\n");
    }
    if (add_office(sys, 1, 10, neighbors1, 2) == ALREADY_EXISTS) {
        printf("Дубликат офиса 1 корректно не добавлен\n");
    }
    printf("\n");
    printf("find_office_by_id\n");
    Office *office = find_office_by_id(sys, 1);
    if (office) {
        printf("Найден офис 1, capacity = %zu, neighbors_count = %zu\n", office->capacity, office->neighbors_count);
    } else {
        printf("Офис 1 не найден\n");
    }
    office = find_office_by_id(sys, 3);
    if (!office) {
        printf("Офис 3 корректно не найден\n");
    }
    printf("\n");
    printf("delete_office\n");
    if (delete_office(sys, 2) == SUCCESS) {
        printf("Офис 2 успешно удалён\n");
    }
    if (delete_office(sys, 2) == NOT_FOUND) {
        printf("Офис 2 корректно повторно удалить нельзя\n");
    }
    office = find_office_by_id(sys, 2);
    if (!office) {
        printf("Офис 2 после удаления не найден\n");
    }
    printf("\n");
    printf("destroy_system\n");
    destroy_system(sys);
    return 0;
}
