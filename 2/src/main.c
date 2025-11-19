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

int main(void) {
    Vector v = create_vector(4, copy_int, delete_int);
    delete_vector(&v);
    return 0;
}
