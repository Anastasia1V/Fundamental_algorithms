#include "liver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

size_t read_liver(const char *input, Liver *livers, size_t size) {
    FILE *file = fopen(input, "r");
    if (file == NULL) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", input);
        return 0;
    }
    size_t count = 0;
    char str[512];
    while (fgets(str, sizeof(str), file) != NULL && count < size) {
        str[strcspn(str, "\r\n")] = '\0';
        char *words[16] = {0};
        size_t count_words = 0;
        char *ptr = strtok(str, " \t");
        while (ptr != NULL && count_words < 16) {
            words[count_words] = ptr;
            count_words = count_words + 1;
            ptr = strtok(NULL, " \t");
        }
        if (count_words < 6) {
            continue;
        }
        unsigned int id = (unsigned int)atoi(words[0]);
        char *last = words[1];
        char *first = words[2];
        char *second = "";
        int date_index = 3;
        int gender_index = 4;
        int earnings_index = 5;
        if (count_words >= 7) {
            if (strchr(words[3], '.') == NULL) {
                second = words[3];
                date_index = 4;
                gender_index = 5;
                earnings_index = 6;
            }
        }
        int day, month, year;
        if (sscanf(words[date_index], "%d.%d.%d", &day, &month, &year) != 3) {
            continue;
        }
        if (gender_index >= (int)count_words || earnings_index >= (int)count_words) {
            continue;
        }
        char gender = words[gender_index][0];
        double earnings = atof(words[earnings_index]);
        livers[count].id = id;
        strncpy(livers[count].last_name, last, sizeof(livers[count].last_name) - 1);
        livers[count].last_name[sizeof(livers[count].last_name) - 1] = '\0';
        strncpy(livers[count].first_name, first, sizeof(livers[count].first_name) - 1);
        livers[count].first_name[sizeof(livers[count].first_name) - 1] = '\0';
        strncpy(livers[count].second_name, second, sizeof(livers[count].second_name) - 1);
        livers[count].second_name[sizeof(livers[count].second_name) - 1] = '\0';
        livers[count].day = day;
        livers[count].month = month;
        livers[count].year = year;
        livers[count].gender = gender;
        livers[count].earnings = earnings;
        count = count + 1;
    }
    fclose(file);
    return count;
}

LinkedList *create_list(void) {
    LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void erase_list(LinkedList *list) {
    Node *node = list->head;
    while (node != NULL) {
        Node *delete = node;
        node = node->next;
        free(delete);
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void delete_list(LinkedList *list) {
    erase_list(list);
    free(list);
}

void push_back_list(LinkedList *list, LIST_TYPE value) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return;
    }
    node->data = value;
    node->next = NULL;
    node->prev = list->tail;
    if (list->tail != NULL) {
        list->tail->next = node;
    }
    list->tail = node;
    if (list->head == NULL) {
        list->head = node;
    }
    list->size = list->size + 1;
}

void push_front_list(LinkedList *list, LIST_TYPE value) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return;
    }
    node->data = value;
    node->prev = NULL;
    node->next = list->head;
    if (list->head != NULL) {
        list->head->prev = node;
    }
    list->head = node;
    if (list->tail == NULL) {
        list->tail = node;
    }
    list->size = list->size + 1;
}

LIST_TYPE pop_back_list(LinkedList *list) {
    LIST_TYPE value = {0};
    if (list->tail == NULL) {
        return value;
    }
    Node *delete = list->tail;
    value = delete->data;
    list->tail = delete->prev;
    if (list->tail != NULL) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }
    free(delete);
    list->size = list->size - 1;
    return value;
}

LIST_TYPE pop_front_list(LinkedList *list) {
    LIST_TYPE value = {0};
    if (list->head == NULL) {
        return value;
    }
    Node *delete = list->head;
    value = delete->data;
    list->head = delete->next;
    if (list->head != NULL) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }
    free(delete);
    list->size = list->size - 1;
    return value;
}

void insert_at_list(LinkedList *list, size_t index, LIST_TYPE value) {
    if (index >= list->size) {
        push_back_list(list, value);
        return;
    }
    Node *n = list->head;
    for (size_t i = 0; i < index; i = i + 1) {
        n = n->next;
    }
    Node *node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return;
    }
    node->data = value;
    node->prev = n->prev;
    node->next = n;
    if (n->prev != NULL) {
        n->prev->next = node;
    } else {
        list->head = node;
    }
    n->prev = node;
    list->size = list->size + 1;
}

void delete_at_list(LinkedList *list, size_t index) {
    if (index >= list->size) {
        return;
    }
    Node *node = list->head;
    for (size_t i = 0; i < index; i = i + 1) {
        node = node->next;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    free(node);
    list->size = list->size - 1;
}

LIST_TYPE get_at_list(const LinkedList *list, size_t index) {
    LIST_TYPE value = {0};
    if (index >= list->size) {
        return value;
    }
    Node *node = list->head;
    for (size_t i = 0; i < index; i = i + 1) {
        node = node->next;
    }
    return node->data;
}

int is_equal_list(const LinkedList *l1, const LinkedList *l2) {
    if (l1->size != l2->size) {
        return 0;
    }
    Node *n1 = l1->head;
    Node *n2 = l2->head;
    while (n1 != NULL && n2 != NULL) {
        if (n1->data.id != n2->data.id) {
            return 0;
        }
        if (strcmp(n1->data.last_name, n2->data.last_name) != 0) {
            return 0;
        }
        if (strcmp(n1->data.first_name, n2->data.first_name) != 0) {
            return 0;
        }
        if (strcmp(n1->data.second_name, n2->data.second_name) != 0) {
            return 0;
        }
        if (n1->data.day != n2->data.day || n1->data.month != n2->data.month || n1->data.year != n2->data.year) {
            return 0;
        }
        if (n1->data.gender != n2->data.gender) {
            return 0;
        }
        if (n1->data.earnings != n2->data.earnings) {
            return 0;
        }
        n1 = n1->next;
        n2 = n2->next;
    }
    return 1;
}

LinkedList *make_linked_list(const Liver *livers, size_t count) {
    LinkedList *list = create_list();
    if (list == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < count; i = i + 1) {
        push_back_list(list, livers[i]);
    }
    return list;
}
