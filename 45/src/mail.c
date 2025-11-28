#include "mail.h"
#include <stdlib.h>
#include <string.h>

static void replace_each_other(int *a, int *b)  {
    if (a == NULL || b == NULL) {
        return;
    }
    int c = *a;
    *a = *b;
    *b = c;
}

static void put_up(Heap *h, size_t index)  {
    if (h == NULL || h->data == NULL) {
        return;
    }
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (h->data[parent] <= h->data[index]) {
            break;
        }
        replace_each_other(&h->data[parent], &h->data[index]);
        index = parent;
    }
}

static void put_down(Heap *h, size_t index)  {
    if (h == NULL || h->data == NULL) {
        return;
    }
    while (1) {
        size_t left = index * 2 + 1;
        size_t right = index * 2 + 2;
        size_t minimal = index;
        if (left < h->size && h->data[left] < h->data[minimal]) {
            minimal = left;
        }
        if (right < h->size && h->data[right] < h->data[minimal]) {
            minimal = right;
        }
        if (minimal == index) {
            break;
        }
        replace_each_other(&h->data[index], &h->data[minimal]);
        index = minimal;
    }
}

Heap create_heap(size_t initial_capacity)  {
    Heap h;
    h.data = NULL;
    h.size = 0;
    h.capacity = 0;
    size_t cap = initial_capacity;
    if (cap == 0) {
        cap = 1;
    }
    h.data = (int*)malloc(cap * sizeof(int));
    if (h.data == NULL) {
        h.capacity = 0;
        h.size = 0;
        return h;
    }
    h.capacity = cap;
    h.size = 0;
    return h;
}

void delete_heap(Heap *h)  {
    if (h == NULL) {
        return;
    }
    if (h->data != NULL) {
        free(h->data);
        h->data = NULL;
    }
    h->size = 0;
    h->capacity = 0;
}

int is_empty_heap(const Heap *h)  {
    if (h == NULL) {
        return 1;
    }
    if (h->size == 0) {
        return 1;
    }
    return 0;
}

size_t size_heap(const Heap *h)  {
    if (h == NULL) {
        return 0;
    }
    return h->size;
}

int peek_heap(const Heap *h)  {
    if (h == NULL || h->data == NULL || h->size == 0) {
        return INT_MIN;
    }
    return h->data[0];
}

void push_heap(Heap *h, int value)  {
    if (h == NULL) {
        return;
    }
    if (h->data == NULL) {
        h->capacity = 1;
        h->data = (int*)malloc(h->capacity * sizeof(int));
        if (h->data == NULL) {
            h->capacity = 0;
            return;
        }
    }
    if (h->size + 1 > h->capacity) {
        size_t new_capacity = h->capacity * 2;
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        int *new_data = (int*)realloc(h->data, new_capacity * sizeof(int));
        if (new_data == NULL) {
            return;
        }
        h->data = new_data;
        h->capacity = new_capacity;
    }
    h->data[h->size] = value;
    size_t index = h->size;
    h->size += 1;
    put_up(h, index);
}

int pop_heap(Heap *h)  {
    if (h == NULL || h->data == NULL || h->size == 0) {
        return INT_MIN;
    }
    int element = h->data[0];
    if (h->size == 1) {
        h->size = 0;
        return element;
    }
    h->data[0] = h->data[h->size - 1];
    h->size = h->size - 1;
    put_down(h, 0);
    return element;
}

Heap build_heap(const int *array, size_t n)  {
    Heap h;
    h.data = NULL;
    h.size = 0;
    h.capacity = 0;
    if (n == 0) {
        return create_heap(1);
    }
    if (array == NULL) {
        return create_heap(1);
    }
    h.data = (int*)malloc(n * sizeof(int));
    if (h.data == NULL) {
        h.size = 0;
        h.capacity = 0;
        return h;
    }
    for (size_t i = 0; i < n; i++) {
        h.data[i] = array[i];
    }
    h.size = n;
    h.capacity = n;
    if (h.size > 1) {
        size_t index = (h.size - 1) / 2;
        while (1) {
            put_down(&h, index);
            if (index == 0) {
                break;
            }
            index = index - 1;
        }
    }
    return h;
}

int is_equal_heap(const Heap *h1, const Heap *h2)  {
    if (h1 == NULL && h2 == NULL) {
        return 1;
    }
    if (h1 == NULL || h2 == NULL) {
        return 0;
    }
    if (h1->size != h2->size) {
        return 0;
    }
    for (size_t index = 0; index < h1->size; index++) {
        if (h1->data[index] != h2->data[index]) {
            return 0;
        }
    }
    return 1;
}
