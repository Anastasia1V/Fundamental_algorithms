#ifndef LIVER_H
#define LIVER_H

#include <stddef.h>
#include <stdint.h>

enum status {
    SUCCESS,
    INVALID_ARGS,
    FILE_ERROR
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

typedef Liver LIST_TYPE;

typedef struct Node {
    LIST_TYPE data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    Node *tail;
    size_t size;
} LinkedList;

LinkedList *create_list(void);
void erase_list(LinkedList *list);
void delete_list(LinkedList *list);
void push_back_list(LinkedList *list, LIST_TYPE value);
void push_front_list(LinkedList *list, LIST_TYPE value);
LIST_TYPE pop_back_list(LinkedList *list);
LIST_TYPE pop_front_list(LinkedList *list);
void insert_at_list(LinkedList *list, size_t index, LIST_TYPE value);
void delete_at_list(LinkedList *list, size_t index);
LIST_TYPE get_at_list(const LinkedList *list, size_t index);
int is_equal_list(const LinkedList *l1, const LinkedList *l2);
void push_stack(LinkedList *stack, LIST_TYPE value);
LIST_TYPE pop_stack(LinkedList *stack);
LIST_TYPE peek_stack(const LinkedList *stack);
void enqueue(LinkedList *queue, LIST_TYPE value);
LIST_TYPE dequeue(LinkedList *queue);
LIST_TYPE peek_queue(const LinkedList *queue);

enum status read_liver(const char *input, Liver *livers, size_t max_count, size_t *count_livers);
LinkedList *make_linked_list(const Liver *livers, size_t count);
void print_liver(const Liver *liver);
void print_list(const LinkedList *list);
enum status find_liver(const LinkedList *list);

#endif
