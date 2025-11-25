#ifndef LIVER_H
#define LIVER_H

#include <stddef.h>
#include <stdint.h>

enum status {
    SUCCESS,
    INVALID_ARGS,
    MEMORY_ERROR
};

typedef struct Liver {
    unsigned int id;
    char last_name[64];
    char first_name[64];
    char second_name[64];
    int day;
    int month;
    int year;
    char gender;
    double earnings;
} Liver;

typedef struct Node {
    Liver data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    Node *tail;
    size_t size;
} LinkedList;

LinkedList create_list(void);
void erase_list(LinkedList *list);
void delete_list(LinkedList *list);
void push_back_list(LinkedList *list, Liver value);
void push_front_list(LinkedList *list, Liver value);
Liver pop_back_list(LinkedList *list);
Liver pop_front_list(LinkedList *list);
void insert_at_list(LinkedList *list, size_t index, Liver value);
void delete_at_list(LinkedList *list, size_t index);
Liver get_at_list(const LinkedList *list, size_t index);
int is_equal_list(const LinkedList *l1, const LinkedList *l2);
void push_stack(LinkedList *stack, Liver value);
Liver pop_stack(LinkedList *stack);
Liver peek_stack(const LinkedList *stack);
void enqueue(LinkedList *queue, Liver value);
Liver dequeue(LinkedList *queue);
Liver peek_queue(const LinkedList *queue);

#endif
