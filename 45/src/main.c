#include "mail.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>
#include <errno.h>

void help(void) {
    printf("Меню:\n");
    printf("0. Выход\n");
    printf("1. Добавить офис\n");
    printf("2. Удалить офис\n");
    printf("3. Создать письмо\n");
    printf("4. Пометить письмо как недоставленное\n");
    printf("5. Доставить письмо вручную (взять письмо)\n");
    printf("6. Записать письма в файл\n");
}

static int parse_int_from_fgets(const char *prompt, int *out) {
    char buf[256];
    char *endptr;
    long val;
    printf("%s", prompt);
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
    errno = 0;
    val = strtol(buf, &endptr, 10);
    if (endptr == buf) return 0;
    while (*endptr == ' ' || *endptr == '\t') endptr++;
    if (*endptr != '\n' && *endptr != '\0') return 0;
    if ((val == LONG_MIN || val == LONG_MAX) && errno == ERANGE) return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *out = (int)val;
    return 1;
}

static int parse_uint_from_fgets(const char *prompt, unsigned int *out) {
    char buf[256];
    char *endptr;
    unsigned long val;
    printf("%s", prompt);
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
    errno = 0;
    val = strtoul(buf, &endptr, 10);
    if (endptr == buf) return 0;
    while (*endptr == ' ' || *endptr == '\t') endptr++;
    if (*endptr != '\n' && *endptr != '\0') return 0;
    if (val > UINT_MAX) return 0;
    *out = (unsigned int)val;
    return 1;
}

static int parse_string_from_fgets(const char *prompt, char *out, size_t out_size) {
    char buf[512];
    printf("%s", prompt);
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
    // copy at most out_size-1 chars
    if (out_size == 0) return 0;
    buf[out_size-1] = '\0';
    strncpy(out, buf, out_size);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <mappings_file> <log_path>\n", argv[0]);
        return 1;
    }
    const char *map_path = argv[1];
    const char *log_path;
    if (argc >= 3) {
        log_path = argv[2];
    } else {
        log_path = "log.txt";
    }
    MailSystem *sys = create_system(log_path);
    if (sys == NULL) {
        fprintf(stderr, "Ошибка: не удалось создать систему почты.\n");
        return 1;
    }
    if (read_file(sys, map_path) != SUCCESS) {
        fprintf(stderr, "Ошибка: не удалось прочитать файл связей %s.\n", map_path);
        destroy_system(sys);
        return 1;
    }
    if (start_delivery_thread(sys) != SUCCESS) {
        fprintf(stderr, "Ошибка: не удалось запустить поток доставки.\n");
        destroy_system(sys);
        return 1;
    }
    help();
    while (1) {
        printf("\n");
        printf("Выберите команду: ");
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }
        int v = (int)strtol(input, NULL, 10);
        if (v == 0) {
            printf("Выход.\n");
            break;
        } else if (v == 1) {
            unsigned int id;
            int capacity;
            int n;
            if (!parse_uint_from_fgets("ID нового офиса: ", &id)) {
                printf("Неверный ввод ID.\n");
                continue;
            }
            if (!parse_int_from_fgets("Вместимость: ", &capacity)) {
                printf("Неверный ввод вместимости.\n");
                continue;
            }
            if (!parse_int_from_fgets("Количество соседей: ", &n)) {
                printf("Неверный ввод количества соседей.\n");
                continue;
            }
            if (n < 0 || n > 1024) {
                printf("Неверное количество соседей (0<=n<=1024)\n");
                continue;
            }
            unsigned int *neighbors = NULL;
            if (n > 0) {
                neighbors = (unsigned int*)malloc((size_t)n * sizeof(unsigned int));
                if (!neighbors) {
                    printf("Ошибка памяти при выделении соседей.\n");
                    continue;
                }
                int ok = 1;
                for (int i = 0; i < n; i++) {
                    char prompt[64];
                    snprintf(prompt, sizeof(prompt), "ID соседа %d: ", i + 1);
                    if (!parse_uint_from_fgets(prompt, &neighbors[i])) {
                        neighbors[i] = 0;
                    }
                }
                if (!ok) {
                    free(neighbors);
                    continue;
                }
            }
            if (add_office(sys, id, (size_t)capacity, neighbors, (size_t)n) != SUCCESS) {
                printf("Ошибка добавления офиса\n");
            }
            else {
                printf("Офис добавлен успешно\n");
            }
            if (neighbors) free(neighbors);
        } else if (v == 2) {
            unsigned int id;
            if (!parse_uint_from_fgets("ID офиса для удаления: ", &id)) {
                printf("Неверный ввод.\n");
                continue;
            }
            if (delete_office(sys, id) != SUCCESS) {
                printf("Ошибка удаления офиса\n");
            } else {
                printf("Офис удалён успешно\n");
            }
        } else if (v == 3) {
            char type[32];
            char data[256];
            int priority;
            unsigned int src;
            unsigned int dst;
            if (!parse_string_from_fgets("Тип письма: ", type, sizeof(type))) {
                printf("Неверный ввод типа.\n");
                continue;
            }
            if (!parse_int_from_fgets("Приоритет (целое): ", &priority)) {
                printf("Неверный ввод приоритета.\n");
                continue;
            }
            if (!parse_uint_from_fgets("От какого офиса (ID): ", &src)) {
                printf("Неверный ввод src.\n");
                continue;
            }
            if (!parse_uint_from_fgets("До какого офиса (ID): ", &dst)) {
                printf("Неверный ввод dst.\n");
                continue;
            }
            if (!parse_string_from_fgets("Данные письма: ", data, sizeof(data))) {
                printf("Неверный ввод данных.\n");
                continue;
            }
            unsigned int mail_id;
            if (create_mail(sys, type, priority, src, dst, data, &mail_id) != SUCCESS) {
                printf("Ошибка создания письма\n");
            } else {
                printf("Письмо %u успешно создано\n", mail_id);
            }
        } else if (v == 4) {
            unsigned int id;
            if (!parse_uint_from_fgets("ID письма для пометки недоставленным: ", &id)) {
                printf("Неверный ввод.\n");
                continue;
            }
            if (mark_mail_undelivered(sys, id) != SUCCESS) {
                printf("Ошибка пометки письма\n");
            } else {
                printf("Письмо %u помечено как недоставленное\n", id);
            }
        } else if (v == 5) {
            unsigned int id;
            if (!parse_uint_from_fgets("ID письма для доставки вручную: ", &id)) {
                printf("Неверный ввод.\n");
                continue;
            }
            if (take_mail(sys, id) != SUCCESS) {
                printf("Ошибка доставки письма\n");
            } else {
                printf("Письмо %u доставлено вручную\n", id);
            }
        } else if (v == 6) {
            char path[256];
            if (!parse_string_from_fgets("Имя файла для сохранения писем: ", path, sizeof(path))) {
                printf("Неверный ввод.\n");
                continue;
            }
            if (mails_to_file(sys, path) != SUCCESS) {
                printf("Ошибка записи писем в файл\n");
            } else {
                printf("Письма успешно записаны в %s\n", path);
            }
        } else {
            printf("Неизвестная команда, попробуйте снова\n");
        }
    }
    stop_delivery_thread(sys);
    destroy_system(sys);
    return 0;
}
