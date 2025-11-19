// #define VECTOR_TYPE int

#include "type.h"
#include "vector.h"
#include <stdio.h>

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
    printf("\nerase_vector\n");
    printf("\nsize: %zu\ncapacity: %zu\n", v.size, v.capacity);
    delete_vector(&v);
    return 0;
}
