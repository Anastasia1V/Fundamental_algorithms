#include "numbers.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum status task(unsigned long long number, unsigned int base, char *out, size_t size) {
    if (out == NULL || size == 0) {
        return INVALID_ARGS;
    }
    if (base < 1 || base > 5) {
        return INVALID_ARGS;
    }
    if (number == 0) {
        out[0] = '0';
        out[1] = '\0';
        return SUCCESS;
    }
    return SUCCESS;
}
