#include "numbers.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

enum status number_to_base(unsigned long long number, unsigned int base, char *out, size_t size) {
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
    unsigned int numbers_bits = (1 << base) - 1;
    unsigned int numbers_max = 0;
    unsigned long long remain = ULLONG_MAX;
    while (remain != 0) {
        remain >>= base;
        numbers_max += 1;
    }
    unsigned int *numbers = malloc(numbers_max * sizeof(unsigned int));
    if (numbers == NULL) {
        return MEMORY_ERROR;
    }
    size_t index = 0;
    unsigned long long n = number;
    while (n != 0 && index < numbers_max) {
        unsigned int digit = (unsigned int)(n & numbers_bits);
        numbers[index] = digit;
        index += 1;
        n = n >> base;
    }
    if (n != 0) {
        free(numbers);
        return MEMORY_ERROR;
    }
    if (index + 1 > size) {
        free(numbers);
        return MEMORY_ERROR;
    }
    size_t out_index = 0;
    while (index > 0) {
        index -= 1;
        unsigned int v = numbers[index];
        char ans;
        if (v < 10) {
            ans = (char)('0' + v);
        }
        else {
            ans = (char)('A' + (v - 10));
        }
        out[out_index] = ans;
        out_index += 1;
    }
    out[out_index] = '\0';
    free(numbers);
    return SUCCESS;
}
