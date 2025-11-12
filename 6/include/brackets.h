#ifndef BRACKETS_H
#define BRACKETS_H

#include <stddef.h>

enum status {
    SUCCESS,
    MEMORY_ERROR
};

int check_brackets(const char *str);

#endif
