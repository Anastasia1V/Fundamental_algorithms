#include "vector.h"
#include <stdlib.h>
#include <math.h>

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

void erase_vector(Vector *v) {
    if (v == NULL) {
        return;
    }
    if (v->data != NULL) {
        for (size_t i = 0; i < v->size; i++) {
            v->DeleteVoidPtr(v->data[i]);
        }
        free(v->data);
        v->data = NULL;
    }
    v->size = 0;
    v->capacity = 0;
}

int is_equal_vector(const Vector *v1, const Vector *v2) {
    if (v1 == NULL && v2 == NULL) {
        return 1;
    }
    if (v1 == NULL || v2 == NULL) {
        return 0;
    }
    if (v1->size != v2->size) {
        return 0;
    }
    for (size_t i = 0; i < v1->size; i++) {
        VECTOR_TYPE el1;
        VECTOR_TYPE el2;
        if (v1->CopyVoidPtr != NULL) {
            el1 = v1->CopyVoidPtr(v1->data[i]);
        } else {
            el1 = v1->data[i];
        }
        if (v2->CopyVoidPtr != NULL) {
            el2 = v2->CopyVoidPtr(v2->data[i]);
        } else {
            el2 = v2->data[i];
        }
        #ifdef VECTOR_TYPE_IS_FLOAT
        if (fabs(el1 - el2) > 1e-9) {
            return 0;
        }
        #else
        if (el1 != el2) {
            return 0;
        }
        #endif
    }
    return 1;
}

void copy_vector(Vector *dest, const Vector *src) {
    if (dest == NULL || src == NULL) {
        return;
    }
    erase_vector(dest);
    if (src->capacity == 0) {
        return;
    }
    dest->data = (VECTOR_TYPE *)malloc(src->capacity * sizeof(VECTOR_TYPE));
    if (dest->data == NULL) {
        dest->size = 0;
        dest->capacity = 0;
        return;
    }
    dest->capacity = src->capacity;
    dest->size = src->size;
    dest->CopyVoidPtr = src->CopyVoidPtr;
    dest->DeleteVoidPtr = src->DeleteVoidPtr;
    for (size_t i = 0; i < src->size; i++) {
        if (src->CopyVoidPtr != NULL) {
            dest->data[i] = src->CopyVoidPtr(src->data[i]);
        } else {
            dest->data[i] = src->data[i];
        }
    }
}

Vector *copy_vector_new(const Vector *src) {
    if (src == NULL) {
        return NULL;
    }
    Vector *v = (Vector *)malloc(sizeof(Vector));
    if (v == NULL) {
        return NULL;
    }
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
    v->CopyVoidPtr = src->CopyVoidPtr;
    v->DeleteVoidPtr = src->DeleteVoidPtr;
    copy_vector(v, src);
    return v;
}

void push_back_vector(Vector *v, VECTOR_TYPE value) {
    if (v == NULL) {
        return;
    }
    if (v->capacity == 0) {
        size_t new_capacity = 1;
        VECTOR_TYPE *new_data = (VECTOR_TYPE *)malloc(new_capacity * sizeof(VECTOR_TYPE));
        if (new_data == NULL) {
            return;
        }
        v->data = new_data;
        v->capacity = new_capacity;
    } else if (v->size == v->capacity) {
        size_t new_capacity = v->capacity * 2;
        VECTOR_TYPE *new_data = (VECTOR_TYPE *)realloc(v->data, new_capacity * sizeof(VECTOR_TYPE));
        if (new_data == NULL) {
            return;
        }
        v->data = new_data;
        v->capacity = new_capacity;
    }
    size_t index = v->size;
    if (v->CopyVoidPtr != NULL) {
        v->data[index] = v->CopyVoidPtr(value);
    } else {
        v->data[index] = value;
    }
    v->size += 1;
}

void delete_at_vector(Vector *v, size_t index) {
    if (v == NULL || v->data == NULL) {
        return;
    }
    if (index < v->size) {
        if (v->DeleteVoidPtr != NULL) {
            v->DeleteVoidPtr(v->data[index]);
        }
        if (index + 1 < v->size) {
            size_t ind = index;
            while (ind + 1 < v->size) {
                v->data[ind] = v->data[ind + 1];
                ind += 1;
            }
        }
        v->size -= 1;
    }
    return;
}

VECTOR_TYPE get_at_vector(const Vector *v, size_t index) {
    if (v == NULL || v->data == NULL) {
        return (VECTOR_TYPE)0;
    }
    if (index < v->size) {
        if (v->CopyVoidPtr != NULL) {
            return v->CopyVoidPtr(v->data[index]);
        } else {
            return v->data[index];
        }
    }
    return (VECTOR_TYPE)0;
}

void delete_vector(Vector *v) {
    if (v == NULL) {
        return;
    }
    if (v->data != NULL) {
        if (v->DeleteVoidPtr != NULL) {
            for (size_t i = 0; i < v->size; i++) {
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
