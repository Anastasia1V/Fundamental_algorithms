#ifndef NUMBERS_H
#define NUMBERS_H

#include <stddef.h>
#include <stdint.h>

enum status {
    SUCCESS,
    INVALID_ARGS,
    MEMORY_ERROR
};

enum status number_to_base(unsigned long long number, unsigned int base, char *out, size_t size);

#endif
