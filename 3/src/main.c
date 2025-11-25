#include "liver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_liver(const Liver *liver) {
    printf("id: %u, Фамилия: %s, Имя: %s, Отчество: %s, Дата: %02d.%02d.%04d, Пол: %c, Доход: %.2f\n",
           liver->id, liver->last_name, liver->first_name, liver->second_name,
           liver->day, liver->month, liver->year, liver->gender, liver->earnings);
}

static void print_list(const LinkedList *list) {
    printf("Список (размер %zu):\n", list->size);
    Node *node = list->head;
    size_t index = 0;
    while (node != NULL) {
        printf("[%zu] ", index);
        print_liver(&node->data);
        node = node->next;
        index = index + 1;
    }
    printf("\n");
}

int main(void) {
    Liver livers[256];
    size_t count = read_liver("3/liver.txt", livers, 256);
    LinkedList *list = make_linked_list(livers, count);
    printf("Список после make_linked_list:\n");
    print_list(list);
    Liver new;
    new.id = 99;
    strcpy(new.last_name, "Фамилия");
    strcpy(new.first_name, "Имя");
    strcpy(new.second_name, "Отчество");
    new.day = 1;
    new.month = 1;
    new.year = 2000;
    new.gender = 'M';
    new.earnings = 12345.67;
    printf("push_back_list\n");
    push_back_list(list, new);
    print_list(list);
    printf("push_front_list\n");
    push_front_list(list, new);
    print_list(list);
    printf("pop_back_list\n");
    LIST_TYPE pop_back = pop_back_list(list);
    print_liver(&pop_back);
    print_list(list);
    printf("pop_front_list\n");
    LIST_TYPE pop_front = pop_front_list(list);
    print_liver(&pop_front);
    print_list(list);
    printf("insert_at_list\n");
    insert_at_list(list, 1, new);
    print_list(list);
    printf("delete_at_list\n");
    delete_at_list(list, 1);
    print_list(list);
    printf("get_at_list\n");
    LIST_TYPE elem = get_at_list(list, 0);
    print_liver(&elem);
    printf("is_equal_list\n");
    LinkedList *copy = make_linked_list(livers, count);
    if (is_equal_list(list, copy)) {
        printf("Списки равны.\n");
    } else {
        printf("Списки не равны.\n");
    }
    printf("erase_list\n");
    erase_list(list);
    print_list(list);
    printf("delete_list\n");
    delete_list(copy);
    return 0;
}
