#include "type.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

static VECTOR_TYPE copy_int(VECTOR_TYPE v) {
    return v;
}
static void delete_int(VECTOR_TYPE v) {
    (void)v;
}

int main() {
    Vector v = create_vector(2, copy_int, delete_int);
    push_back_vector(&v, 1);
    push_back_vector(&v, 2);
    push_back_vector(&v, 3);
    printf("Вектор:\n");
    for (size_t i = 0; i < v.size; i++) {
        printf("%d ", get_at_vector(&v, i));
    }
    printf("\n");
    printf("\ndelete_at_vector\n");
    delete_at_vector(&v, 1);
    for (size_t i = 0; i < v.size; i++) {
        VECTOR_TYPE el = get_at_vector(&v, i);
        printf("v[%zu] = %d\n", i, el);
    }
    printf("\nsize: %zu\ncapacity: %zu\n", v.size, v.capacity);
    erase_vector(&v);
    printf("erase_vector");
    printf("\nsize: %zu\ncapacity: %zu\n", v.size, v.capacity);
    delete_vector(&v);
    printf("\nis_equal_vector\n");
    Vector v1 = create_vector(2, copy_int, delete_int);
    push_back_vector(&v1, 1);
    push_back_vector(&v1, 2);
    Vector v2 = create_vector(2, copy_int, delete_int);
    push_back_vector(&v2, 1);
    push_back_vector(&v2, 2);
    Vector v3 = create_vector(2, copy_int, delete_int);
    push_back_vector(&v3, 2);
    push_back_vector(&v3, 3);
    printf("v1 == v2: %d\n", is_equal_vector(&v1, &v2));
    printf("v1 == v3: %d\n", is_equal_vector(&v1, &v3));
    delete_vector(&v1);
    delete_vector(&v2);
    delete_vector(&v3);
    Vector v4 = create_vector(2, copy_int, delete_int);
    push_back_vector(&v4, 1);
    push_back_vector(&v4, 2);
    push_back_vector(&v4, 3);
    Vector v5 = create_vector(0, copy_int, delete_int);
    printf("\ncopy_vector\n");
    copy_vector(&v5, &v4);
    printf("\nВектор v5:\n");
    for (size_t i = 0; i < v5.size; i++) {
        printf("v5[%zu] = %d\n", i, get_at_vector(&v5, i));
    }
    delete_vector(&v4);
    delete_vector(&v5);
    Vector v6 = create_vector(2, copy_int, delete_int);
    push_back_vector(&v6, 6);
    push_back_vector(&v6, 7);
    Vector *v7 = copy_vector_new(&v6);
    if (v7 != NULL) {
        printf("\nВектор v7:\n");
        for (size_t i = 0; i < v7->size; i++) {
            printf("v7[%zu] = %d\n", i, get_at_vector(v7, i));
        }
        delete_vector(v7);
        free(v7);
    }
    delete_vector(&v6);
    return 0;
}
