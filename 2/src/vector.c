#include "vector.h"
#include <stdlib.h>

Vector create_vector(size_t initial_capacity, VECTOR_TYPE (*CopyFunc)(VECTOR_TYPE), void (*DeleteFunc)(VECTOR_TYPE)) {
    Vector v;
    v.data = NULL;
    v.size = 0;
    v.capacity = 0;
    v.CopyVoidPtr = CopyFunc;
    v.DeleteVoidPtr = DeleteFunc;
    if (initial_capacity == 0) {
        return v;
    }
    v.data = (VECTOR_TYPE *)malloc(initial_capacity * sizeof(VECTOR_TYPE));
    if (v.data == NULL) {
        v.capacity = 0;
        return v;
    }
    v.capacity = initial_capacity;
    return v;
}

void delete_vector(Vector *v) {
    if (v == NULL) {
        return;
    }
    if (v->data != NULL) {
        if (v->DeleteVoidPtr != NULL) {
            size_t i;
            for (i = 0; i < v->size; i += 1) {
                v->DeleteVoidPtr(v->data[i]);
            }
        }
        free(v->data);
        v->data = NULL;
    }
    v->size = 0;
    v->capacity = 0;
    v->CopyVoidPtr = NULL;
    v->DeleteVoidPtr = NULL;
}
