#include "liver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

enum status read_liver(const char *input, Liver *livers, size_t max_count, size_t *count_livers) {
    if (input == NULL || livers == NULL || count_livers == NULL || max_count == 0) {
        return INVALID_ARGS;
    }
    FILE *file = fopen(input, "r");
    if (file == NULL) {
        return FILE_ERROR;
    }
    size_t count = 0;
    char str[512];
    while (fgets(str, sizeof(str), file) != NULL) {
        if (count >= max_count) {
            break;
        }
        str[strcspn(str, "\r\n")] = '\0';
        char *words[16];
        size_t words_count = 0;
        for (size_t i = 0; i < 16; i++) {
            words[i] = NULL;
        }
        char *ptr = strtok(str, " \t");
        while (ptr != NULL && words_count < 16) {
            words[words_count] = ptr;
            words_count += 1;
            ptr = strtok(NULL, " \t");
        }
        if (words_count < 6) {
            continue;
        }
        unsigned int id = (unsigned int)atoi(words[0]);
        char *last = words[1];
        char *first = words[2];
        char *second = "";
        int day_index = 3;
        int gender_index = 4;
        int earnings_index = 5;
        if (words_count >= 7) {
            if (strchr(words[3], '.') == NULL) {
                second = words[3];
                day_index = 4;
                gender_index = 5;
                earnings_index = 6;
            }
        }
        int day, month, year;
        if (sscanf(words[day_index], "%d.%d.%d", &day, &month, &year) != 3) {
            continue;
        }
        if (gender_index >= (int)words_count || earnings_index >= (int)words_count) {
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
        count += 1;
    }
    fclose(file);
    *count_livers = count;
    return SUCCESS;
}

void print_liver(const Liver *liver) {
    if (liver == NULL) {
        return;
    }
    printf("id: %u, Фамилия: %s, Имя: %s, Отчество: %s, Дата: %02d.%02d.%04d, Пол: %c, Доход: %.2f\n",
           liver->id, liver->last_name, liver->first_name, liver->second_name,
           liver->day, liver->month, liver->year, liver->gender, liver->earnings);
}

void print_list(const LinkedList *list) {
    if (list == NULL) {
        printf("Список (NULL)\n");
        return;
    }
    printf("Список (размер %zu):\n", list->size);
    Node *node = list->head;
    size_t index = 0;
    while (node != NULL) {
        printf("[%zu] ", index);
        print_liver(&node->data);
        node = node->next;
        index += 1;
    }
    printf("\n");
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
    if (list == NULL) {
        return;
    }
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
    if (list == NULL) {
        return;
    }
    erase_list(list);
    free(list);
}

void push_back_list(LinkedList *list, LIST_TYPE value) {
    if (list == NULL) {
        return;
    }
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
    list->size += 1;
}

void push_front_list(LinkedList *list, LIST_TYPE value) {
    if (list == NULL) {
        return;
    }
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
    list->size += 1;
}

LIST_TYPE pop_back_list(LinkedList *list) {
    LIST_TYPE value = {0};
    if (list == NULL) {
        return value;
    }
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
    list->size -= 1;
    return value;
}

LIST_TYPE pop_front_list(LinkedList *list) {
    LIST_TYPE value = {0};
    if (list == NULL) {
        return value;
    }
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
    list->size -= 1;
    return value;
}

void insert_at_list(LinkedList *list, size_t index, LIST_TYPE value) {
    if (list == NULL) {
        return;
    }
    if (index >= list->size) {
        push_back_list(list, value);
        return;
    }
    Node *n = list->head;
    for (size_t i = 0; i < index; i++) {
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
    list->size += 1;
}

void delete_at_list(LinkedList *list, size_t index) {
    if (list == NULL) {
        return;
    }
    if (index >= list->size) {
        return;
    }
    Node *node = list->head;
    for (size_t i = 0; i < index; i++) {
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
    list->size -= 1;
}

LIST_TYPE get_at_list(const LinkedList *list, size_t index) {
    LIST_TYPE value = {0};
    if (list == NULL) {
        return value;
    }
    if (index >= list->size) {
        return value;
    }
    Node *node = list->head;
    for (size_t i = 0; i < index; i++) {
        node = node->next;
    }
    return node->data;
}

int is_equal_list(const LinkedList *l1, const LinkedList *l2) {
    if (l1 == NULL || l2 == NULL) {
        return 0;
    }
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
        if (fabs(n1->data.earnings - n2->data.earnings) > 1e-9) {
            return 0;
        }
        n1 = n1->next;
        n2 = n2->next;
    }
    return 1;
}

static int compare_age(const Liver *l1, const Liver *l2) {
    if (l1 == NULL || l2 == NULL) {
        return 0;
    }
    if (l1->year > l2->year) {
        return -1;
    } else if (l1->year < l2->year) {
        return 1;
    }
    if (l1->month > l2->month) {
        return -1;
    } else if (l1->month < l2->month) {
            return 1;
    }
    if (l1->day > l2->day) {
        return -1;
    } else if (l1->day < l2->day) {
        return 1;
    }
    return 0;
}

LinkedList *make_linked_list(const Liver *livers, size_t count) {
    if (livers == NULL) {
        return NULL;
    }
    LinkedList *list = create_list();
    if (list == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < count; i++) {
        LIST_TYPE value = livers[i];
        size_t index = 0;
        Node *node = list->head;
        while (node != NULL && compare_age(&value, &node->data) >= 0) {
            node = node->next;
            index += 1;
        }
        insert_at_list(list, index, value);
    }
    return list;
}

void push_stack(LinkedList *stack, LIST_TYPE value) {
    if (stack == NULL) {
        return;
    }
    push_back_list(stack, value);
}

LIST_TYPE pop_stack(LinkedList *stack) {
    if (stack == NULL) {
        LIST_TYPE empty = {0};
        return empty;
    }
    return pop_back_list(stack);
}

LIST_TYPE peek_stack(const LinkedList *stack) {
    LIST_TYPE empty = {0};
    if (stack == NULL) {
        return empty;
    }
    if (stack->tail != NULL) {
        return stack->tail->data;
    }
    return empty;
}

void enqueue(LinkedList *queue, LIST_TYPE value) {
    if (queue == NULL) {
        return;
    }
    push_back_list(queue, value);
}

LIST_TYPE dequeue(LinkedList *queue) {
    if (queue == NULL) {
        LIST_TYPE empty = {0};
        return empty;
    }
    return pop_front_list(queue);
}

LIST_TYPE peek_queue(const LinkedList *queue) {
    LIST_TYPE empty = {0};
    if (queue == NULL) {
        return empty;
    }
    if (queue->head != NULL) {
        return queue->head->data;
    }
    return empty;
}

enum status find_liver(const LinkedList *list) {
    if (list == NULL) {
        return INVALID_ARGS;
    }
    printf("По каким данным искать:\n");
    printf("1. id\n");
    printf("2. Фамилия\n");
    printf("3. Имя\n");
    printf("4. Отчество\n");
    printf("5. Дата (формат 21.12.1991)\n");
    printf("6. День\n");
    printf("7. Месяц\n");
    printf("8. Год\n");
    printf("9. Пол\n");
    printf("10. Доход\n");
    char input[128];
    printf("Введите номер: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return INVALID_ARGS;
    }
    input[strcspn(input, "\r\n")] = '\0';
    if (input[0] == '\0') {
        return INVALID_ARGS;
    }
    int v = atoi(input);
    if (v < 1 || v > 10) {
        printf("Некорректное число.\n");
        return INVALID_ARGS;
    }
    char input2[128];
    unsigned int search_id = 0;
    char search_last_name[64] = {0};
    char search_first_name[64] = {0};
    char search_second_name[64] = {0};
    int search_day = 0;
    int search_month = 0;
    int search_year = 0;
    char search_gender = '\0';
    double search_earnings = 0.0;
    if (v == 1) {
        printf("Введите id: ");
        if (fgets(input2, sizeof(input2), stdin) == NULL) {
            return INVALID_ARGS;
        }
        input2[strcspn(input2, "\r\n")] = '\0';
        search_id = (unsigned int)atoi(input2);
    } else if (v == 2) {
        printf("Введите фамилию: ");
        if (fgets(input2, sizeof(input2), stdin) == NULL) {
            return INVALID_ARGS;
        }
        input2[strcspn(input2, "\r\n")] = '\0';
        strncpy(search_last_name, input2, sizeof(search_last_name) - 1);
        search_last_name[sizeof(search_last_name) - 1] = '\0';
    } else if (v == 3) {
        printf("Введите имя: ");
        if (fgets(input2, sizeof(input2), stdin) == NULL) {
            return INVALID_ARGS;
        }
        input2[strcspn(input2, "\r\n")] = '\0';
        strncpy(search_first_name, input2, sizeof(search_first_name) - 1);
        search_first_name[sizeof(search_first_name) - 1] = '\0';
    } else if (v == 4) {
        printf("Введите отчество: ");
        if (fgets(input2, sizeof(input2), stdin) == NULL) {
            return INVALID_ARGS;
        }
        input2[strcspn(input2, "\r\n")] = '\0';
        strncpy(search_second_name, input2, sizeof(search_second_name) - 1);
        search_second_name[sizeof(search_second_name) - 1] = '\0';
    } else if (v == 5) {
        printf("Введите дату (формат 21.12.1991): \n");
        if (fgets(input2, sizeof(input2), stdin) == NULL) {
            return INVALID_ARGS;
        }
        input2[strcspn(input2, "\r\n")] = '\0';
        if (sscanf(input2, "%d.%d.%d", &search_day, &search_month, &search_year) != 3) {
            printf("Неправильный формат даты.\n");
            return INVALID_ARGS;
        }
    } else if (v == 6) {
        printf("Введите день: ");
        if (fgets(input2, sizeof(input2), stdin) == NULL) {
            return INVALID_ARGS;
        }
        input2[strcspn(input2, "\r\n")] = '\0';
        search_day = atoi(input2);
    } else if (v == 7) {
        printf("Введите месяц: ");
        if (fgets(input2, sizeof(input2), stdin) == NULL) {
            return INVALID_ARGS;
        }
        input2[strcspn(input2, "\r\n")] = '\0';
        search_month = atoi(input2);
    } else if (v == 8) {
        printf("Введите год: ");
        if (fgets(input2, sizeof(input2), stdin) == NULL) {
            return INVALID_ARGS;
        }
        input2[strcspn(input2, "\r\n")] = '\0';
        search_year = atoi(input2);
    } else if (v == 9) {
        printf("Введите пол (M или W): ");
        if (fgets(input2, sizeof(input2), stdin) == NULL) {
            return INVALID_ARGS;
        }
        input2[strcspn(input2, "\r\n")] = '\0';
        if (input2[0] != '\0') {
            search_gender = (char)toupper((unsigned char)input2[0]);
        }
    } else if (v == 10) {
        printf("Введите доход: ");
        if (fgets(input2, sizeof(input2), stdin) == NULL) {
            return INVALID_ARGS;
        }
        input2[strcspn(input2, "\r\n")] = '\0';
        search_earnings = atof(input2);
    }
    printf("Найдено:\n");
    Node *node = list->head;
    size_t found = 0;
    while (node != NULL) {
        const Liver *liver = &node->data;
        int find = 0;
        if (v == 1) {
            if (liver->id == search_id) {
                find = 1;
            }
        } else if (v == 2) {
            if (strcmp(liver->last_name, search_last_name) == 0) {
                find = 1;
            }
        } else if (v == 3) {
            if (strcmp(liver->first_name, search_first_name) == 0) {
                find = 1;
            }
        } else if (v == 4) {
            if (strcmp(liver->second_name, search_second_name) == 0) {
                find = 1;
            }
        } else if (v == 5) {
            if (liver->day == search_day && liver->month == search_month && liver->year == search_year) {
                find = 1;
            }
        } else if (v == 6) {
            if (liver->day == search_day) {
                find = 1;
            }
        } else if (v == 7) {
            if (liver->month == search_month) {
                find = 1;
            }
        } else if (v == 8) {
            if (liver->year == search_year) {
                find = 1;
            }
        } else if (v == 9) {
            if (liver->gender == search_gender) {
                find = 1;
            }
        } else if (v == 10) {
            double number = liver->earnings - search_earnings;
            if (number < 0.0) {
                number = number * -1;
            }
            if (number <= 1e-9) {
                find = 1;
            }
        }
        if (find) {
            print_liver(liver);
            found += 1;
        }
        node = node->next;
    }
    if (found == 0) {
        printf("Не найдено.\n");
    } else {
        printf("Найдено %zu жителей.\n", found);
    }
    return SUCCESS;
}

static int is_alpha_str(const char *s) {
    if (s == NULL) {
        return 0;
    }
    if (s[0] == '\0') {
        return 0;
    }
    for (size_t i = 0; s[i] != '\0'; i++) {
        unsigned char c = (unsigned char)s[i];
        if (!isalpha(c)) {
            return 0;
        }
    }
    return 1;
}

static int is_digits_str(const char *s) {
    if (s == NULL) {
        return 0;
    }
    if (s[0] == '\0') {
        return 0;
    }
    for (size_t i = 0; s[i] != '\0'; i++) {
        unsigned char c = (unsigned char)s[i];
        if (!isdigit(c)) {
            return 0;
        }
    }
    return 1;
}

static int is_valid_date_str(const char *s) {
    if (s == NULL) {
        return 0;
    }
    if (s[0] == '\0') {
        return 0;
    }
    int dot_count = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == '.') {
            dot_count += 1;
        } else if (!isdigit(c)) {
            return 0;
        }
    }
    if (dot_count != 2) {
        return 0;
    }
    return 1;
}

static int is_valid_gender_str(const char *s) {
    if (s == NULL) {
        return 0;
    }
    if (s[0] == '\0') {
        return 0;
    }
    char c = (char)toupper((unsigned char)s[0]);
    if (c == 'M' || c == 'W') {
        return 1;
    }
    return 0;
}

static int is_valid_number_str(const char *s) {
    if (s == NULL) {
        return 0;
    }
    if (s[0] == '\0') {
        return 0;
    }
    int dot_count = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == '.') {
            dot_count += 1;
            if (dot_count > 1) {
                return 0;
            }
            if (i == 0) {
                return 0;
            }
            if (s[i + 1] == '\0') {
                return 0;
            }
        } else if (!isdigit(c)) {
            return 0;
        }
    }
    return 1;
}

Node *get_liver_by_id(LinkedList *list, unsigned int id) {
    if (list == NULL) {
        return NULL;
    }
    Node *node = list->head;
    while (node != NULL) {
        if (node->data.id == id) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

enum status edit_liver(LinkedList *list) {
    if (list == NULL) {
        return INVALID_ARGS;
    }
    printf("Введите id: ");
    char input[128];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return INVALID_ARGS;
    }
    input[strcspn(input, "\r\n")] = '\0';
    if (input[0] == '\0') {
        return INVALID_ARGS;
    }
    unsigned int id = (unsigned int)atoi(input);
    Node *node = get_liver_by_id(list, id);
    if (node == NULL) {
        printf("Житель с id = %u не найден.\n", id);
        return INVALID_ARGS;
    }
    printf("Что изменить:\n");
    printf("1. id\n");
    printf("2. Фамилия\n");
    printf("3. Имя\n");
    printf("4. Отчество\n");
    printf("5. Дата (формат 21.12.1991)\n");
    printf("6. День\n");
    printf("7. Месяц\n");
    printf("8. Год\n");
    printf("9. Пол\n");
    printf("10. Доход\n");
    printf("Введите номер: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return INVALID_ARGS;
    }
    input[strcspn(input, "\r\n")] = '\0';
    if (input[0] == '\0') {
        return INVALID_ARGS;
    }
    int v = atoi(input);
    if (v < 1 || v > 10) {
        printf("Некорректное число.\n");
        return INVALID_ARGS;
    }
    char user_input[128];
    if (v == 1) {
        printf("Введите новый id: ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            return INVALID_ARGS;
        }
        user_input[strcspn(user_input, "\r\n")] = '\0';
        if (!is_digits_str(user_input)) {
            printf("Ошибка: id должен содержать только цифры и не быть пустым.\n");
            return INVALID_ARGS;
        }
        unsigned int new_id = (unsigned int)atoi(user_input);
        if (get_liver_by_id(list, new_id) != NULL && new_id != node->data.id) {
            printf("Житель с id = %u уже существует.\n", new_id);
            return INVALID_ARGS;
        }
        node->data.id = new_id;
    } else if (v == 2) {
        printf("Введите новую фамилию: ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            return INVALID_ARGS;
        }
        user_input[strcspn(user_input, "\r\n")] = '\0';
        if (!is_alpha_str(user_input)) {
            printf("Ошибка: фамилия должна содержать только буквы и не быть пустой.\n");
            return INVALID_ARGS;
        }
        strncpy(node->data.last_name, user_input, sizeof(node->data.last_name) - 1);
        node->data.last_name[sizeof(node->data.last_name) - 1] = '\0';
    } else if (v == 3) {
        printf("Введите новое имя: ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            return INVALID_ARGS;
        }
        user_input[strcspn(user_input, "\r\n")] = '\0';
        if (!is_alpha_str(user_input)) {
            printf("Ошибка: имя должно содержать только буквы и не быть пустым.\n");
            return INVALID_ARGS;
        }
        strncpy(node->data.first_name, user_input, sizeof(node->data.first_name) - 1);
        node->data.first_name[sizeof(node->data.first_name) - 1] = '\0';
    } else if (v == 4) {
        printf("Введите новое отчество: ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            return INVALID_ARGS;
        }
        user_input[strcspn(user_input, "\r\n")] = '\0';
        if (user_input[0] != '\0' && !is_alpha_str(user_input)) {
            printf("Ошибка: отчество должно содержать только буквы или быть пустым.\n");
            return INVALID_ARGS;
        }
        strncpy(node->data.second_name, user_input, sizeof(node->data.second_name) - 1);
        node->data.second_name[sizeof(node->data.second_name) - 1] = '\0';
    } else if (v == 5) {
        printf("Введите новую дату (формат 21.12.1991): ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            return INVALID_ARGS;
        }
        user_input[strcspn(user_input, "\r\n")] = '\0';
        if (!is_valid_date_str(user_input)) {
            printf("Неправильный формат даты (только цифры и две точки, пример 21.12.1991).\n");
            return INVALID_ARGS;
        }
        int d, m, y;
        if (sscanf(user_input, "%d.%d.%d", &d, &m, &y) != 3) {
            printf("Неправильный формат даты.\n");
            return INVALID_ARGS;
        }
        node->data.day = d;
        node->data.month = m;
        node->data.year = y;
    } else if (v == 6) {
        printf("Введите новый день: ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            return INVALID_ARGS;
        }
        user_input[strcspn(user_input, "\r\n")] = '\0';
        if (!is_digits_str(user_input)) {
            printf("Ошибка: день должен содержать только цифры.\n");
            return INVALID_ARGS;
        }
        node->data.day = atoi(user_input);
    } else if (v == 7) {
        printf("Введите новый месяц: ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            return INVALID_ARGS;
        }
        user_input[strcspn(user_input, "\r\n")] = '\0';
        if (!is_digits_str(user_input)) {
            printf("Ошибка: месяц должен содержать только цифры.\n");
            return INVALID_ARGS;
        }
        node->data.month = atoi(user_input);
    } else if (v == 8) {
        printf("Введите новый год: ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            return INVALID_ARGS;
        }
        user_input[strcspn(user_input, "\r\n")] = '\0';
        if (!is_digits_str(user_input)) {
            printf("Ошибка: год должен содержать только цифры.\n");
            return INVALID_ARGS;
        }
        node->data.year = atoi(user_input);
    } else if (v == 9) {
        printf("Введите новый пол (M или W): ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            return INVALID_ARGS;
        }
        user_input[strcspn(user_input, "\r\n")] = '\0';
        if (!is_valid_gender_str(user_input)) {
            printf("Ошибка: пол должен быть M или W.\n");
            return INVALID_ARGS;
        }
        node->data.gender = (char)toupper((unsigned char)user_input[0]);
    } else if (v == 10) {
        printf("Введите новый доход (только цифры и точка для дробной части): ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            return INVALID_ARGS;
        }
        user_input[strcspn(user_input, "\r\n")] = '\0';
        if (!is_valid_number_str(user_input)) {
            printf("Ошибка: доход должен быть числом (например 12345 или 12345.67).\n");
            return INVALID_ARGS;
        }
        node->data.earnings = atof(user_input);
    }
    printf("Житель изменён.\n");
    return SUCCESS;
}

enum status delete_liver(LinkedList *list) {
    if (list == NULL) {
        return INVALID_ARGS;
    }
    printf("Введите id: ");
    char input[128];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return INVALID_ARGS;
    }
    input[strcspn(input, "\r\n")] = '\0';
    if (!is_digits_str(input)) {
        return INVALID_ARGS;
    }
    unsigned int id = (unsigned int)atoi(input);
    Node *node = get_liver_by_id(list, id);
    if (node == NULL) {
        printf("Житель с id = %u не найден.\n", id);
        return INVALID_ARGS;
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
    list->size -= 1;
    printf("Житель с id = %u удалён.\n", id);
    return SUCCESS;
}

enum status add_liver(LinkedList *list) {
    if (list == NULL) {
        return INVALID_ARGS;
    }
    printf("Введите новый id: ");
    char input[128];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return INVALID_ARGS;
    }
    input[strcspn(input, "\r\n")] = '\0';
    if (!is_digits_str(input)) {
        printf("Ошибка: id должен содержать только цифры и не быть пустым.\n");
        return INVALID_ARGS;
    }
    unsigned int id = (unsigned int)atoi(input);
    if (get_liver_by_id(list, id) != NULL) {
        printf("Житель с id = %u уже существует.\n", id);
        return INVALID_ARGS;
    }
    Liver new_liver;
    new_liver.id = id;
    char user_input[128];
    printf("Введите фамилию: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        return INVALID_ARGS;
    }
    user_input[strcspn(user_input, "\r\n")] = '\0';
    if (!is_alpha_str(user_input)) {
        printf("Ошибка: фамилия должна содержать только буквы и не быть пустой.\n");
        return INVALID_ARGS;
    }
    strncpy(new_liver.last_name, user_input, sizeof(new_liver.last_name) - 1);
    new_liver.last_name[sizeof(new_liver.last_name) - 1] = '\0';
    printf("Введите имя: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        return INVALID_ARGS;
    }
    user_input[strcspn(user_input, "\r\n")] = '\0';
    if (!is_alpha_str(user_input)) {
        printf("Ошибка: имя должно содержать только буквы и не быть пустым.\n");
        return INVALID_ARGS;
    }
    strncpy(new_liver.first_name, user_input, sizeof(new_liver.first_name) - 1);
    new_liver.first_name[sizeof(new_liver.first_name) - 1] = '\0';
    printf("Введите отчество: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        return INVALID_ARGS;
    }
    user_input[strcspn(user_input, "\r\n")] = '\0';
    if (user_input[0] != '\0' && !is_alpha_str(user_input)) {
        printf("Ошибка: отчество должно содержать только буквы или быть пустым.\n");
        return INVALID_ARGS;
    }
    strncpy(new_liver.second_name, user_input, sizeof(new_liver.second_name) - 1);
    new_liver.second_name[sizeof(new_liver.second_name) - 1] = '\0';
    printf("Введите дату рождения (формат 21.12.1991): ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        return INVALID_ARGS;
    }
    user_input[strcspn(user_input, "\r\n")] = '\0';
    if (sscanf(user_input, "%d.%d.%d", &new_liver.day, &new_liver.month, &new_liver.year) != 3) {
        printf("Неправильный формат даты.\n");
        return INVALID_ARGS;
    }
    printf("Введите пол (M или W): ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        return INVALID_ARGS;
    }
    user_input[strcspn(user_input, "\r\n")] = '\0';
    if (!is_valid_gender_str(user_input)) {
        printf("Ошибка: пол должен быть M или W.\n");
        return INVALID_ARGS;
    }
    new_liver.gender = (char)toupper((unsigned char)user_input[0]);
    printf("Введите средний доход: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        return INVALID_ARGS;
    }
    user_input[strcspn(user_input, "\r\n")] = '\0';
    if (!is_valid_number_str(user_input)) {
        printf("Ошибка: доход должен быть числом.\n");
        return INVALID_ARGS;
    }
    new_liver.earnings = atof(user_input);
    size_t index = 0;
    Node *node = list->head;
    while (node != NULL && compare_age(&new_liver, &node->data) >= 0) {
        node = node->next;
        index += 1;
    }
    insert_at_list(list, index, new_liver);
    printf("Житель с id = %u добавлен.\n", id);
    return SUCCESS;
}
