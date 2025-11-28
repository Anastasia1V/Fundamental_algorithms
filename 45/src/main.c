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
    return 0;
}
