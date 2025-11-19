#include "type.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static VECTOR_TYPE copy_str(VECTOR_TYPE v) {
    if (v == NULL) {
        return NULL;
    }
    size_t length = strlen(v) + 1;
    char *copy = (char *)malloc(length);
    if (copy != NULL) {
        memcpy(copy, v, length);
    }
    return copy;
}

static void delete_str(VECTOR_TYPE v) {
    free(v);
}

int main() {
    Vector v = create_vector(2, copy_str, delete_str);
    push_back_vector(&v, "one");
    push_back_vector(&v, "two");
    push_back_vector(&v, "three");
    printf("Вектор:\n");
    for (size_t i = 0; i < v.size; i++) {
        printf("%s ", get_at_vector(&v, i));
    }
    printf("\n");
    printf("\ndelete_at_vector\n");
    delete_at_vector(&v, 1);
    for (size_t i = 0; i < v.size; i++) {
        VECTOR_TYPE el = get_at_vector(&v, i);
        printf("v[%zu] = %s\n", i, el);
    }
    printf("\nsize: %zu\ncapacity: %zu\n", v.size, v.capacity);
    erase_vector(&v);
    printf("erase_vector\n");
    printf("size: %zu\ncapacity: %zu\n", v.size, v.capacity);
    delete_vector(&v);
    printf("\nis_equal_vector\n");
    Vector v1 = create_vector(2, copy_str, delete_str);
    push_back_vector(&v1, "one");
    push_back_vector(&v1, "two");
    Vector v2 = create_vector(2, copy_str, delete_str);
    push_back_vector(&v2, "one");
    push_back_vector(&v2, "two");
    Vector v3 = create_vector(2, copy_str, delete_str);
    push_back_vector(&v3, "two");
    push_back_vector(&v3, "three");
    printf("v1 == v2: %d\n", is_equal_vector(&v1, &v2));
    printf("v1 == v3: %d\n", is_equal_vector(&v1, &v3));
    delete_vector(&v1);
    delete_vector(&v2);
    delete_vector(&v3);
    Vector v4 = create_vector(2, copy_str, delete_str);
    push_back_vector(&v4, "one");
    push_back_vector(&v4, "two");
    push_back_vector(&v4, "three");
    Vector v5 = create_vector(0, copy_str, delete_str);
    printf("\ncopy_vector\n");
    copy_vector(&v5, &v4);
    printf("\nВектор v5:\n");
    for (size_t i = 0; i < v5.size; i++) {
        printf("v5[%zu] = %s\n", i, get_at_vector(&v5, i));
    }
    delete_vector(&v4);
    delete_vector(&v5);
    Vector v6 = create_vector(2, copy_str, delete_str);
    push_back_vector(&v6, "six");
    push_back_vector(&v6, "seven");
    Vector *v7 = copy_vector_new(&v6);
    if (v7 != NULL) {
        printf("\nВектор v7:\n");
        for (size_t i = 0; i < v7->size; i++) {
            printf("v7[%zu] = %s\n", i, get_at_vector(v7, i));
        }
        delete_vector(v7);
        free(v7);
    }
    delete_vector(&v6);
    return 0;
}
