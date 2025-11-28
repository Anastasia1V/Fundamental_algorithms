#include "mail.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void) {
    MailSystem *sys2 = create_system("mail_log.txt");
    if (sys2 != NULL) {
        printf("Система успешно создана\n");
    }
    printf("\n");
    printf("add_office\n");
    unsigned int neighbors12[] = {2};
    unsigned int neighbors22[] = {1};
    enum status st;
    st = add_office(sys2, 1, 10, neighbors12, 1);
    if (st == SUCCESS) {
        printf("Офис 1 добавлен\n");
    }
    st = add_office(sys2, 2, 5, neighbors22, 1);
    if (st == SUCCESS) {
        printf("Офис 2 добавлен\n");
    }
    st = add_office(sys2, 1, 5, NULL, 0);
    if (st == ALREADY_EXISTS) {
        printf("Дубликат офиса 1 корректно не добавлен\n");
    }
    printf("\n");
    printf("find_office_by_id\n");
    Office *office2 = find_office_by_id(sys2, 1);
    if (office2 != NULL) {
        printf("Найден офис 1, capacity = %zu, neighbors_count = %zu\n", office2->capacity, office2->neighbors_count);
    }
    office2 = find_office_by_id(sys2, 3);
    if (office2 == NULL) {
        printf("Офис 3 корректно не найден\n");
    }
    printf("\n");
    printf("create_mail\n");
    unsigned int mail1_id, mail2_id;
    st = create_mail(sys2, "Letter", 5, 1, 2, "Hello", &mail1_id);
    if (st == SUCCESS) {
        printf("Письмо 1 создано, id = %u\n", mail1_id);
    }
    st = create_mail(sys2, "Package", 10, 1, 2, "Box", &mail2_id);
    if (st == SUCCESS) {
        printf("Письмо 2 создано, id = %u\n", mail2_id);
    }
    printf("\n");
    printf("get_mail_by_id\n");
    Mail *m = get_mail_by_id(sys2, mail1_id);
    if (m != NULL) {
        printf("Найдено письмо id = %u, type = %s, state = %d\n", m->id, m->type, m->state);
    }
    printf("\n");
    printf("mark_mail_undelivered\n");
    st = mark_mail_undelivered(sys2, mail1_id);
    if (st == SUCCESS) {
        printf("Письмо id = %u помечено недоставленным\n", mail1_id);
    }
    printf("\n");
    printf("take_mail\n");
    st = take_mail(sys2, mail2_id);
    if (st == SUCCESS) {
        printf("Письмо id = %u доставлено\n", mail2_id);
    }
    printf("\n");
    printf("mails_to_file\n");
    st = mails_to_file(sys2, "out_mails.txt");
    if (st == SUCCESS) {
        printf("Письма записаны в out_mails.txt\n");
    }
    printf("\n");
    printf("delete_office\n");
    st = delete_office(sys2, 2);
    if (st == SUCCESS) {
        printf("Офис 2 успешно удалён\n");
    }
    st = delete_office(sys2, 2);
    if (st == NOT_FOUND) {
        printf("Офис 2 корректно повторно удалить нельзя\n");
    }
    printf("\n");
    printf("deliver_mails\n");
    create_mail(sys2, "Letter", 5, 1, 2, "Hello", &mail1_id);
    create_mail(sys2, "Package", 10, 1, 2, "Box", &mail2_id);
    st = deliver_mails(sys2);
    if (st == SUCCESS) {
        printf("Письма обработаны и переданы по офисам\n");
    }
    for (size_t i = 0; i < sys2->mails_count; i++) {
        Mail *m = sys2->mails[i];
        printf("Mail id = %u, state = %d, src = %u, dst = %u\n", m->id, m->state, m->src_office, m->dst_office);
    }
    enum status st2 = read_file(sys2, "45/mappings.txt");
    if (st2 != SUCCESS) {
        printf("Ошибка при чтении маппингов: %d\n", st2);
    } else {
        printf("Маппинги успешно загружены\n");
    }
    printf("\n");
    printf("destroy_system\n");
    destroy_system(sys2);
}
