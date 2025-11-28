#ifndef MAIL_H
#define MAIL_H

#include <stddef.h>
#include <limits.h>
#include <stdio.h>

enum status {
    SUCCESS,
    INVALID_ARGS,
    FILE_ERROR,
    MEMORY_ERROR,
    NOT_FOUND,
    ALREADY_EXISTS
};

enum mail_state {
    MAIL_IN_TRANSIT,
    MAIL_DELIVERED,
    MAIL_UNDELIVERED
};

typedef struct Heap {
    int *data;
    size_t size;
    size_t capacity;
} Heap;

typedef struct Mail {
    unsigned int id;
    char type[32];
    enum mail_state state;
    int priority;
    unsigned int src_office;
    unsigned int dst_office;
    char tech_data[256];
} Mail;

typedef struct Office {
    unsigned int id;
    size_t capacity;
    Heap mailbox;
    unsigned int *neighbors;
    size_t neighbors_count;
    int available;
} Office;


typedef struct MailSystem {
    Office *offices;
    size_t offices_count;
    Mail **mails;
    size_t mails_count;
    size_t mails_capacity;
    FILE *log;
    char log_path[256];
} MailSystem;

Heap create_heap(size_t initial_capacity);
void delete_heap(Heap *h);
int is_empty_heap(const Heap *h);
size_t size_heap(const Heap *h);
int peek_heap(const Heap *h);
void push_heap(Heap *h, int value);
int pop_heap(Heap *h);
Heap build_heap(const int *array, size_t n);
int is_equal_heap(const Heap *h1, const Heap *h2);

MailSystem *create_system(const char *log_path);
void destroy_system(MailSystem *sys);
enum status read_file(MailSystem *sys, const char *path);
enum status add_office(MailSystem *sys, unsigned int id, size_t capacity, const unsigned int *neighbors, size_t neighbors_count);
enum status delete_office(MailSystem *sys, unsigned int id);
enum status create_mail(MailSystem *sys, const char *type, int priority, unsigned int src_office, unsigned int dst_office, const char *tech_data, unsigned int *out_mail_id);
enum status mark_mail_undelivered(MailSystem *sys, unsigned int mail_id);
enum status take_mail(MailSystem *sys, unsigned int mail_id);
enum status mails_to_file(MailSystem *sys, const char *out_path);
enum status time(MailSystem *sys);
Office *find_office_by_id(const MailSystem *sys, unsigned int id);
Mail *get_mail_by_id(const MailSystem *sys, unsigned int mail_id);
int encode_heap_key(int priority, unsigned int mail_id);
unsigned int decode_mail_id_from_key(int key);
enum status deliver_mails(MailSystem *sys);

#endif
