#include "mail.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void) {
    Heap h = create_heap(2);
    if (h.data == NULL && h.capacity == 0) {
        fprintf(stderr, "Ошибка при создании кучи.\n");
        return 1;
    }
    printf("push_heap\n");
    push_heap(&h, 5);
    push_heap(&h, 3);
    push_heap(&h, 7);
    push_heap(&h, 1);
    printf("peek_heap\n");
    int p = peek_heap(&h);
    if (p == INT_MIN) {
        printf("peek_heap вернул ошибку (куча пуста или NULL).\n");
    } else {
        printf("Корень: %d\n", p);
    }
    printf("\n");
    printf("size_heap: %zu\n", size_heap(&h));
    printf("is_empty_heap: %d\n", is_empty_heap(&h));
    printf("pop_heap последовательность:\n");
    while (!is_empty_heap(&h)) {
        int v = pop_heap(&h);
        if (v == INT_MIN) {
            printf("pop_heap вернул ошибку (пустая куча).\n");
            break;
        }
        printf("%d ", v);
    }
    printf("\n\n");
    printf("Попытка pop из пустой кучи:\n");
    int v = pop_heap(&h);
    if (v == INT_MIN) {
        printf("pop_heap вернул ошибку (пустая куча).\n");
    } else {
        printf("pop_heap вернул %d\n", v);
    }
    printf("\n");
    printf("build_heap\n");
    int arr1[] = {4, 10, 3, 5, 1};
    Heap h2 = build_heap(arr1, sizeof(arr1) / sizeof(arr1[0]));
    if (h2.data == NULL && h2.capacity == 0) {
        fprintf(stderr, "Ошибка при build_heap.\n");
        delete_heap(&h);
        return 1;
    }
    printf("pop_heap из h2:\n");
    while (!is_empty_heap(&h2)) {
        int v = pop_heap(&h2);
        if (v == INT_MIN) {
            printf("pop_heap вернул ошибку\n");
            break;
        }
        printf("%d ", v);
    }
    printf("\n");
    printf("\n");
    printf("build_heap\n");
    int arr2[] = {1, 4, 3, 5, 10};
    Heap h3 = build_heap(arr2, sizeof(arr2) / sizeof(arr2[0]));
    Heap h4 = build_heap(arr1, sizeof(arr1) / sizeof(arr1[0]));
    if (h3.data == NULL || h4.data == NULL) {
        fprintf(stderr, "Ошибка при создании куч.\n");
        delete_heap(&h);
        delete_heap(&h2);
        delete_heap(&h3);
        delete_heap(&h4);
        return 1;
    }
    printf("is_equal_heap\n");
    if (is_equal_heap(&h3, &h4)) {
        printf("h3 и h4 равны (лексикографически).\n");
    } else {
        printf("h3 и h4 не равны (лексикографически).\n");
    }
    printf("\n");
    printf("Проверка size_heap для h3:\n");
    printf("size_heap(h3) = %zu\n", size_heap(&h3));
    printf("\n");
    printf("Проверка is_empty_heap для h3:\n");
    printf("is_empty_heap(h3) = %d\n", is_empty_heap(&h3));
    delete_heap(&h);
    delete_heap(&h2);
    delete_heap(&h3);
    delete_heap(&h4);
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
    printf("create_system\n");
    MailSystem *sys2 = create_system("mail_log.txt");
    if (sys2 != NULL) {
        printf("Система успешно создана\n");
    }
    printf("\n");
    printf("add_office\n");
    unsigned int neighbors12[] = {2};
    unsigned int neighbors22[] = {1};
    enum status st;
    st = add_office(sys2, 1, 10, neighbors12, 1);
    if (st == SUCCESS) {
        printf("Офис 1 добавлен\n");
    }
    st = add_office(sys2, 2, 5, neighbors22, 1);
    if (st == SUCCESS) {
        printf("Офис 2 добавлен\n");
    }
    st = add_office(sys2, 1, 5, NULL, 0);
    if (st == ALREADY_EXISTS) {
        printf("Дубликат офиса 1 корректно не добавлен\n");
    }
    printf("\n");
    printf("find_office_by_id\n");
    Office *office2 = find_office_by_id(sys2, 1);
    if (office2 != NULL) {
        printf("Найден офис 1, capacity = %zu, neighbors_count = %zu\n", office2->capacity, office2->neighbors_count);
    }
    office2 = find_office_by_id(sys2, 3);
    if (office2 == NULL) {
        printf("Офис 3 корректно не найден\n");
    }
    printf("\n");
    printf("create_mail\n");
    unsigned int mail1_id, mail2_id;
    st = create_mail(sys2, "Letter", 5, 1, 2, "Hello", &mail1_id);
    if (st == SUCCESS) {
        printf("Письмо 1 создано, id = %u\n", mail1_id);
    }
    st = create_mail(sys2, "Package", 10, 1, 2, "Box", &mail2_id);
    if (st == SUCCESS) {
        printf("Письмо 2 создано, id = %u\n", mail2_id);
    }
    printf("\n");
    printf("get_mail_by_id\n");
    Mail *m = get_mail_by_id(sys2, mail1_id);
    if (m != NULL) {
        printf("Найдено письмо id = %u, type = %s, state = %d\n", m->id, m->type, m->state);
    }
    printf("\n");
    printf("mark_mail_undelivered\n");
    st = mark_mail_undelivered(sys2, mail1_id);
    if (st == SUCCESS) {
        printf("Письмо id = %u помечено недоставленным\n", mail1_id);
    }
    printf("\n");
    printf("take_mail\n");
    st = take_mail(sys2, mail2_id);
    if (st == SUCCESS) {
        printf("Письмо id = %u доставлено\n", mail2_id);
    }
    printf("\n");
    printf("mails_to_file\n");
    st = mails_to_file(sys2, "out_mails.txt");
    if (st == SUCCESS) {
        printf("Письма записаны в out_mails.txt\n");
    }
    printf("\n");
    printf("delete_office\n");
    st = delete_office(sys2, 2);
    if (st == SUCCESS) {
        printf("Офис 2 успешно удалён\n");
    }
    st = delete_office(sys2, 2);
    if (st == NOT_FOUND) {
        printf("Офис 2 корректно повторно удалить нельзя\n");
    }
    printf("\n");
    printf("destroy_system\n");
    destroy_system(sys2);
    return 0;
}
