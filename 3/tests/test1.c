#include "liver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
     Liver livers[256];
    size_t count = 0;
    enum status st = read_liver("3/liver.txt", livers, 256, &count);
    if (st != SUCCESS) {
        fprintf(stderr, "Ошибка при чтении файла: %d\n", st);
        return 1;
    }
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
    printf("\n");
    printf("is_equal_list\n");
    LinkedList *copy = make_linked_list(livers, count);
    if (is_equal_list(list, copy)) {
        printf("Списки равны.\n");
    } else {
        printf("Списки не равны.\n");
    }
    printf("\n");
    printf("erase_list\n");
    erase_list(list);
    print_list(list);
    printf("delete_list\n");
    delete_list(copy);
    printf("\n");
    LinkedList *stack = create_list();
    printf("push_stack\n");
    push_stack(stack, new);
    push_stack(stack, livers[0]);
    push_stack(stack, livers[1]);
    print_list(stack);
    printf("peek_stack\n");
    LIST_TYPE stack2 = peek_stack(stack);
    print_liver(&stack2);
    printf("\n");
    printf("pop_stack\n");
    LIST_TYPE stack3 = pop_stack(stack);
    print_liver(&stack3);
    print_list(stack);
    erase_list(stack);
    delete_list(stack);
    LinkedList *queue = create_list();
    printf("\n");
    printf("enqueue\n");
    enqueue(queue, new);
    enqueue(queue, livers[0]);
    enqueue(queue, livers[1]);
    print_list(queue);
    printf("peek_queue\n");
    LIST_TYPE queue2 = peek_queue(queue);
    print_liver(&queue2);
    printf("\n");
    printf("dequeue\n");
    LIST_TYPE deq = dequeue(queue);
    print_liver(&deq);
    print_list(queue);
    erase_list(queue);
    delete_list(queue);
    return 0;
}
