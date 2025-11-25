#include "liver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    FILE *file = fopen("3/liver.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Ошибка: не удалось открыть файл liver.txt\n");
        return 1;
    }
    Liver livers[256];
    size_t count = 0;
    char line[512];
    while (fgets(line, sizeof(line), file) != NULL && count < 256) {
        line[strcspn(line, "\r\n")] = '\0';
        char *words[16] = {0};
        size_t count_words = 0;
        char *ptr = strtok(line, " \t");
        while (ptr != NULL && count_words < 16) {
            words[count_words] = ptr;
            count_words += 1;
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
        int day = 0;
        int month = 0;
        int year = 0;
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
        count += 1;
    }
    fclose(file);
    for (size_t i = 0; i < count; i++) {
        printf("%u %s %s", livers[i].id, livers[i].last_name, livers[i].first_name);
        if (livers[i].second_name[0] != '\0') {
            printf(" %s", livers[i].second_name);
        }
        printf(" %02d.%02d.%04d %c %.2f\n", 
               livers[i].day, livers[i].month, livers[i].year, 
               livers[i].gender, livers[i].earnings);
    }
    return 0;
}
