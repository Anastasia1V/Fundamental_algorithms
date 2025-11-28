#include "mail.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

static void replace_each_other(int *a, int *b) {
    if (a == NULL || b == NULL) {
        return;
    }
    int c = *a;
    *a = *b;
    *b = c;
}

static void put_up(Heap *h, size_t index) {
    if (h == NULL || h->data == NULL) {
        return;
    }
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (h->data[parent] <= h->data[index]) {
            break;
        }
        replace_each_other(&h->data[parent], &h->data[index]);
        index = parent;
    }
}

static void put_down(Heap *h, size_t index) {
    if (h == NULL || h->data == NULL) {
        return;
    }
    while (1) {
        size_t left = index * 2 + 1;
        size_t right = index * 2 + 2;
        size_t minimal = index;
        if (left < h->size && h->data[left] < h->data[minimal]) {
            minimal = left;
        }
        if (right < h->size && h->data[right] < h->data[minimal]) {
            minimal = right;
        }
        if (minimal == index) {
            break;
        }
        replace_each_other(&h->data[index], &h->data[minimal]);
        index = minimal;
    }
}

Heap create_heap(size_t initial_capacity) {
    Heap h;
    h.data = NULL;
    h.size = 0;
    h.capacity = 0;
    size_t cap = initial_capacity;
    if (cap == 0) {
        cap = 1;
    }
    h.data = (int*)malloc(cap * sizeof(int));
    if (h.data == NULL) {
        h.capacity = 0;
        h.size = 0;
        return h;
    }
    h.capacity = cap;
    h.size = 0;
    return h;
}

void delete_heap(Heap *h) {
    if (h == NULL) {
        return;
    }
    if (h->data != NULL) {
        free(h->data);
        h->data = NULL;
    }
    h->size = 0;
    h->capacity = 0;
}

int is_empty_heap(const Heap *h) {
    if (h == NULL) {
        return 1;
    }
    if (h->size == 0) {
        return 1;
    }
    return 0;
}

size_t size_heap(const Heap *h) {
    if (h == NULL) {
        return 0;
    }
    return h->size;
}

int peek_heap(const Heap *h) {
    if (h == NULL || h->data == NULL || h->size == 0) {
        return INT_MIN;
    }
    return h->data[0];
}

void push_heap(Heap *h, int value) {
    if (h == NULL) {
        return;
    }
    if (h->data == NULL) {
        h->capacity = 1;
        h->data = (int*)malloc(h->capacity * sizeof(int));
        if (h->data == NULL) {
            h->capacity = 0;
            return;
        }
    }
    if (h->size + 1 > h->capacity) {
        size_t new_capacity = h->capacity * 2;
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        int *new_data = (int*)realloc(h->data, new_capacity * sizeof(int));
        if (new_data == NULL) {
            return;
        }
        h->data = new_data;
        h->capacity = new_capacity;
    }
    h->data[h->size] = value;
    size_t index = h->size;
    h->size += 1;
    put_up(h, index);
}

int pop_heap(Heap *h) {
    if (h == NULL || h->data == NULL || h->size == 0) {
        return INT_MIN;
    }
    int element = h->data[0];
    if (h->size == 1) {
        h->size = 0;
        return element;
    }
    h->data[0] = h->data[h->size - 1];
    h->size = h->size - 1;
    put_down(h, 0);
    return element;
}

Heap build_heap(const int *array, size_t n) {
    Heap h;
    h.data = NULL;
    h.size = 0;
    h.capacity = 0;
    if (n == 0) {
        return create_heap(1);
    }
    if (array == NULL) {
        return create_heap(1);
    }
    h.data = (int*)malloc(n * sizeof(int));
    if (h.data == NULL) {
        h.size = 0;
        h.capacity = 0;
        return h;
    }
    for (size_t i = 0; i < n; i++) {
        h.data[i] = array[i];
    }
    h.size = n;
    h.capacity = n;
    if (h.size > 1) {
        size_t index = (h.size - 1) / 2;
        while (1) {
            put_down(&h, index);
            if (index == 0) {
                break;
            }
            index = index - 1;
        }
    }
    return h;
}

int is_equal_heap(const Heap *h1, const Heap *h2) {
    if (h1 == NULL && h2 == NULL) {
        return 1;
    }
    if (h1 == NULL || h2 == NULL) {
        return 0;
    }
    if (h1->size != h2->size) {
        return 0;
    }
    for (size_t index = 0; index < h1->size; index++) {
        if (h1->data[index] != h2->data[index]) {
            return 0;
        }
    }
    return 1;
}

MailSystem *create_system(const char *log_path) {
    if (log_path == NULL) {
        return NULL;
    }
    MailSystem *sys = (MailSystem*)malloc(sizeof(MailSystem));
    if (!sys) {
        return NULL;
    }
    sys->offices = NULL;
    sys->offices_count = 0;
    sys->mails = NULL;
    sys->mails_count = 0;
    sys->mails_capacity = 0;
    sys->log = fopen(log_path, "w");
    if (!sys->log) {
        free(sys);
        return NULL;
    }
    strncpy(sys->log_path, log_path, sizeof(sys->log_path)-1);
    sys->log_path[sizeof(sys->log_path)-1] = '\0';
    pthread_mutex_init(&sys->lock, NULL);
    sys->delivery_running = 0;
    return sys;
}

void destroy_system(MailSystem *sys) {
    if (!sys) {
        return;
    }
    pthread_mutex_lock(&sys->lock);
    if (sys->delivery_running) {
        sys->delivery_running = 0;
        pthread_mutex_unlock(&sys->lock);
        pthread_join(sys->delivery_tid, NULL);
        pthread_mutex_lock(&sys->lock);
    }
    if (sys->offices) {
        for (size_t i = 0; i < sys->offices_count; i++) {
            delete_heap(&sys->offices[i].mailbox);
            if (sys->offices[i].neighbors) {
                free(sys->offices[i].neighbors);
            }
        }
        free(sys->offices);
    }
    if (sys->mails) {
        for (size_t i = 0; i < sys->mails_count; i++) {
            free(sys->mails[i]);
        }
        free(sys->mails);
    }
    if (sys->log) {
        fclose(sys->log);
    }
    pthread_mutex_unlock(&sys->lock);
    pthread_mutex_destroy(&sys->lock);
    free(sys);
}

Office *find_office_by_id(const MailSystem *sys, unsigned int id) {
    if (!sys) {
        return NULL;
    }
    for (size_t i = 0; i < sys->offices_count; i++) {
        if (sys->offices[i].id == id) {
            return &sys->offices[i];
        }
    }
    return NULL;
}

enum status add_office(MailSystem *sys, unsigned int id, size_t capacity,
                       const unsigned int *neighbors, size_t neighbors_count) {
    if (!sys) {
        return INVALID_ARGS;
    }
    pthread_mutex_lock(&sys->lock);
    if (find_office_by_id(sys, id)) {
        pthread_mutex_unlock(&sys->lock);
        return ALREADY_EXISTS;
    }
    Office *new_offices = (Office*)realloc(sys->offices, (sys->offices_count + 1) * sizeof(Office));
    if (!new_offices) {
        pthread_mutex_unlock(&sys->lock);
        return MEMORY_ERROR;
    }
    sys->offices = new_offices;
    Office *office = &sys->offices[sys->offices_count];
    office->id = id;
    office->capacity = capacity;
    office->available = 1;
    office->mailbox = create_heap(capacity);
    if (neighbors_count > 0) {
        office->neighbors = (unsigned int*)malloc(neighbors_count * sizeof(unsigned int));
        if (!office->neighbors) {
            pthread_mutex_unlock(&sys->lock);
            return MEMORY_ERROR;
        }
        for (size_t i = 0; i < neighbors_count; i++) {
            office->neighbors[i] = neighbors[i];
        }
        office->neighbors_count = neighbors_count;
    } else {
        office->neighbors = NULL;
        office->neighbors_count = 0;
    }
    sys->offices_count += 1;
    pthread_mutex_unlock(&sys->lock);
    return SUCCESS;
}

enum status delete_office(MailSystem *sys, unsigned int id) {
    if (!sys) {
        return INVALID_ARGS;
    }
    pthread_mutex_lock(&sys->lock);
    size_t index = 0;
    int found = 0;
    for (size_t i = 0; i < sys->offices_count; i++) {
        if (sys->offices[i].id == id) {
            index = i;
            found = 1;
            break;
        }
    }
    if (!found) {
        pthread_mutex_unlock(&sys->lock);
        return NOT_FOUND;
    }
    delete_heap(&sys->offices[index].mailbox);
    if (sys->offices[index].neighbors) {
        free(sys->offices[index].neighbors);
    }
    for (size_t i = index; i < sys->offices_count - 1; i++) {
        sys->offices[i] = sys->offices[i+1];
    }
    sys->offices_count -= 1;
    if (sys->offices_count == 0) {
        free(sys->offices);
        sys->offices = NULL;
    } else {
        Office *tmp = (Office*)realloc(sys->offices, sys->offices_count * sizeof(Office));
        if (tmp) sys->offices = tmp;
    }
    for (size_t i = 0; i < sys->offices_count; i++) {
        size_t k = 0;
        for (size_t j = 0; j < sys->offices[i].neighbors_count; j++) {
            if (sys->offices[i].neighbors[j] == id) continue;
            sys->offices[i].neighbors[k++] = sys->offices[i].neighbors[j];
        }
        if (k != sys->offices[i].neighbors_count) {
            if (k == 0) {
                free(sys->offices[i].neighbors);
                sys->offices[i].neighbors = NULL;
                sys->offices[i].neighbors_count = 0;
            } else {
                unsigned int *tmpn = (unsigned int*)realloc(sys->offices[i].neighbors, k * sizeof(unsigned int));
                if (tmpn) {
                    sys->offices[i].neighbors = tmpn;
                    sys->offices[i].neighbors_count = k;
                } else {
                    sys->offices[i].neighbors_count = k;
                }
            }
        }
    }
    pthread_mutex_unlock(&sys->lock);
    return SUCCESS;
}

int office_exists(const MailSystem *sys, unsigned int id) {
    if (!sys) {
        return 0;
    }
    for (size_t i = 0; i < sys->offices_count; i++) {
        if (sys->offices[i].id == id) {
            return 1;
        }
    }
    return 0;
}

int encode_heap_key(int priority, unsigned int mail_id) {
    int number = (priority << 16) | (mail_id & 0xFFFF);
    return number;
}

unsigned int decode_mail_id_from_key(int key) {
    int id = key & 0xFFFF;
    return id;
}

int decode_priority_from_key(int key) {
    int pr = (key >> 16) & 0xFFFF;
    return pr;
}

static unsigned int generate_mail_id() {
    static unsigned int counter = 0;
    unsigned int id = counter;
    counter += 1;
    return id;
}

Mail *get_mail_by_id(const MailSystem *sys, unsigned int mail_id) {
    if (sys == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < sys->mails_count; i++) {
        if (sys->mails[i]->id == mail_id) {
            return sys->mails[i];
        }
    }
    return NULL;
}

enum status create_mail(MailSystem *sys, const char *type, int priority, unsigned int src_office, 
    unsigned int dst_office, const char *tech_data, unsigned int *out_mail_id) {
    if (sys == NULL || type == NULL || tech_data == NULL) {
        return INVALID_ARGS;
    }
    pthread_mutex_lock(&sys->lock);
    Office *src = find_office_by_id(sys, src_office);
    Office *dst = find_office_by_id(sys, dst_office);
    if (src == NULL || dst == NULL) {
        pthread_mutex_unlock(&sys->lock);
        return NOT_FOUND;
    }
    if (src->mailbox.size >= src->capacity) {
        pthread_mutex_unlock(&sys->lock);
        return MEMORY_ERROR;
    }
    if (sys->mails_count + 1 > sys->mails_capacity) {
        size_t new_capacity;
        if (sys->mails_capacity == 0) {
            new_capacity = 4;
        } else {
            new_capacity = sys->mails_capacity * 2;
        }
        Mail **new_array = (Mail**)realloc(sys->mails, new_capacity * sizeof(Mail*));
        if (new_array == NULL) {
            pthread_mutex_unlock(&sys->lock);
            return MEMORY_ERROR;
        }
        sys->mails = new_array;
        sys->mails_capacity = new_capacity;
    }
    Mail *m = (Mail*)malloc(sizeof(Mail));
    if (m == NULL) {
        pthread_mutex_unlock(&sys->lock);
        return MEMORY_ERROR;
    }
    m->id = generate_mail_id();
    strncpy(m->type, type, sizeof(m->type) - 1);
    m->type[sizeof(m->type) - 1] = '\0';
    m->state = MAIL_IN_TRANSIT;
    m->priority = priority;
    m->src_office = src_office;
    m->dst_office = dst_office;
    strncpy(m->tech_data, tech_data, sizeof(m->tech_data) - 1);
    m->tech_data[sizeof(m->tech_data) - 1] = '\0';
    sys->mails[sys->mails_count] = m;
    sys->mails_count = sys->mails_count + 1;
    int key = encode_heap_key(priority, m->id);
    push_heap(&src->mailbox, key);
    if (out_mail_id != NULL) {
        *out_mail_id = m->id;
    }
    if (sys->log != NULL) {
        fprintf(sys->log, "CREATE_MAIL: id = %u, type = %s, priority = %d, src = %u, dst = %u\n",
                m->id, m->type, m->priority, m->src_office, m->dst_office);
        fflush(sys->log);
    }
    pthread_mutex_unlock(&sys->lock);
    return SUCCESS;
}

enum status mark_mail_undelivered(MailSystem *sys, unsigned int mail_id) {
    pthread_mutex_lock(&sys->lock);
    Mail *m = get_mail_by_id(sys, mail_id);
    if (m == NULL) {
        pthread_mutex_unlock(&sys->lock);
        return NOT_FOUND;
    }
    m->state = MAIL_UNDELIVERED;
    if (sys->log != NULL) {
        fprintf(sys->log, "MARK_UNDELIVERED: id = %u\n", mail_id);
        fflush(sys->log);
    }
    pthread_mutex_unlock(&sys->lock);
    return SUCCESS;
}

enum status take_mail(MailSystem *sys, unsigned int mail_id) {
    pthread_mutex_lock(&sys->lock);
    Mail *m = get_mail_by_id(sys, mail_id);
    if (m == NULL) {
        pthread_mutex_unlock(&sys->lock);
        return NOT_FOUND;
    }
    Office *dst = find_office_by_id(sys, m->dst_office);
    if (dst == NULL) {
        pthread_mutex_unlock(&sys->lock);
        return NOT_FOUND;
    }
    m->state = MAIL_DELIVERED;
    if (sys->log != NULL) {
        fprintf(sys->log, "TAKE_MAIL: id = %u, delivered at office %u\n", mail_id, dst->id);
        fflush(sys->log);
    }
    for (size_t i = 0; i < sys->mails_count; i++) {
        if (sys->mails[i]->id == mail_id) {
            free(sys->mails[i]);
            for (size_t j = i; j < sys->mails_count - 1; j++) {
                sys->mails[j] = sys->mails[j + 1];
            }
            sys->mails_count = sys->mails_count - 1;
            break;
        }
    }
    pthread_mutex_unlock(&sys->lock);
    return SUCCESS;
}

enum status mails_to_file(MailSystem *sys, const char *out_path) {
    if (sys == NULL || out_path == NULL) {
        return INVALID_ARGS;
    }
    pthread_mutex_lock(&sys->lock);
    FILE *file = fopen(out_path, "w");
    if (file == NULL) {
        pthread_mutex_unlock(&sys->lock);
        return FILE_ERROR;
    }
    for (size_t i = 0; i < sys->mails_count; i++) {
        Mail *m = sys->mails[i];
        fprintf(file, "id = %u, type = %s, state = %d, priority = %d, src = %u, dst = %u, data = %s\n",
                m->id, m->type, m->state, m->priority,
                m->src_office, m->dst_office, m->tech_data);
    }
    fclose(file);
    pthread_mutex_unlock(&sys->lock);
    return SUCCESS;
}

static enum status send_mail_to_neighbor(MailSystem *sys, unsigned int mail_id, unsigned int current_office) {
    if (sys == NULL) {
        return INVALID_ARGS;
    }
    Mail *m = get_mail_by_id(sys, mail_id);
    if (m == NULL) {
        return NOT_FOUND;
    }
    Office *current = find_office_by_id(sys, current_office);
    if (current == NULL) {
        return NOT_FOUND;
    }
    Office *dst = find_office_by_id(sys, m->dst_office);
    if (dst == NULL) {
        return NOT_FOUND;
    }
    if (current_office == m->dst_office) {
        m->state = MAIL_DELIVERED;
        if (sys->log != NULL) {
            fprintf(sys->log, "MAIL_DELIVERED: id = %u at office %u\n", m->id, dst->id);
            fflush(sys->log);
        }
        for (size_t i = 0; i < sys->mails_count; i++) {
            if (sys->mails[i]->id == m->id) {
                free(sys->mails[i]);
                for (size_t j = i; j < sys->mails_count - 1; j++) {
                    sys->mails[j] = sys->mails[j + 1];
                }
                sys->mails_count = sys->mails_count - 1;
                break;
            }
        }
        return SUCCESS;
    }
    for (size_t i = 0; i < current->neighbors_count; i++) {
        unsigned int neighbor_id = current->neighbors[i];
        Office *neighbor = find_office_by_id(sys, neighbor_id);
        if (neighbor == NULL) {
            continue;
        }
        if (neighbor->mailbox.size < neighbor->capacity) {
            int key = encode_heap_key(m->priority, m->id);
            push_heap(&neighbor->mailbox, key);
            if (sys->log != NULL) {
                fprintf(sys->log, "MAIL_MOVED: id = %u from office %u to office %u\n",
                        m->id, current_office, neighbor_id);
                fflush(sys->log);
            }
            return SUCCESS;
        }
    }
    m->state = MAIL_UNDELIVERED;
    if (sys->log != NULL) {
        fprintf(sys->log, "MAIL_UNDELIVERED: id = %u at office %u\n", m->id, current_office);
        fflush(sys->log);
    }
    return SUCCESS;
}

enum status deliver_mails(MailSystem *sys) {
    if (sys == NULL) {
        return INVALID_ARGS;
    }
    pthread_mutex_lock(&sys->lock);
    for (size_t i = 0; i < sys->offices_count; i++) {
        Office *office = &sys->offices[i];
        if (office->mailbox.size == 0) continue;
        int key = pop_heap(&office->mailbox);
        if (key == INT_MIN) continue;
        unsigned int mail_id = decode_mail_id_from_key(key);
        send_mail_to_neighbor(sys, mail_id, office->id);
    }
    pthread_mutex_unlock(&sys->lock);
    return SUCCESS;
}

static void sleep_200ms(void) {
    clock_t start = clock();
    while ((clock() - start) < CLOCKS_PER_SEC / 5) 
    {
    }
}

static void *delivery_thread_func(void *arg) {
    MailSystem *sys = (MailSystem*)arg;
    while (1) {
        pthread_mutex_lock(&sys->lock);
        int run = sys->delivery_running;
        pthread_mutex_unlock(&sys->lock);
        if (!run) break;
        deliver_mails(sys);
        sleep_200ms();
    }
    return NULL;
}

enum status start_delivery_thread(MailSystem *sys) {
    if (sys == NULL) return INVALID_ARGS;
    pthread_mutex_lock(&sys->lock);
    if (sys->delivery_running) {
        pthread_mutex_unlock(&sys->lock);
        return SUCCESS;
    }
    sys->delivery_running = 1;
    if (pthread_create(&sys->delivery_tid, NULL, delivery_thread_func, sys) != 0) {
        sys->delivery_running = 0;
        pthread_mutex_unlock(&sys->lock);
        return FILE_ERROR;
    }
    pthread_mutex_unlock(&sys->lock);
    return SUCCESS;
}

enum status stop_delivery_thread(MailSystem *sys) {
    if (sys == NULL) return INVALID_ARGS;
    pthread_mutex_lock(&sys->lock);
    if (!sys->delivery_running) {
        pthread_mutex_unlock(&sys->lock);
        return SUCCESS;
    }
    sys->delivery_running = 0;
    pthread_mutex_unlock(&sys->lock);
    pthread_join(sys->delivery_tid, NULL);
    return SUCCESS;
}

enum status read_file(MailSystem *sys, const char *path) {
    if (sys == NULL || path == NULL) {
        return INVALID_ARGS;
    }
    pthread_mutex_lock(&sys->lock);
    FILE *file = fopen(path, "r");
    if (!file) {
        pthread_mutex_unlock(&sys->lock);
        return FILE_ERROR;
    }
    unsigned int id1, id2;
    while (fscanf(file, "%u %u", &id1, &id2) == 2) {
        Office *office1 = find_office_by_id(sys, id1);
        Office *office2 = find_office_by_id(sys, id2);
        if (!office1) {
            pthread_mutex_unlock(&sys->lock);
            if (add_office(sys, id1, 5, NULL, 0) != SUCCESS) {
                fclose(file);
                return MEMORY_ERROR;
            }
            pthread_mutex_lock(&sys->lock);
            office1 = find_office_by_id(sys, id1);
        }
        if (!office2) {
            pthread_mutex_unlock(&sys->lock);
            if (add_office(sys, id2, 5, NULL, 0) != SUCCESS) {
                fclose(file);
                return MEMORY_ERROR;
            }
            pthread_mutex_lock(&sys->lock);
            office2 = find_office_by_id(sys, id2);
        }
        int exists = 0;
        for (size_t i = 0; i < office1->neighbors_count; i++) {
            if (office1->neighbors[i] == id2) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            unsigned int *new_neighbors = (unsigned int*)realloc(office1->neighbors, (office1->neighbors_count + 1) * sizeof(unsigned int));
            if (!new_neighbors) {
                fclose(file);
                pthread_mutex_unlock(&sys->lock);
                return MEMORY_ERROR;
            }
            office1->neighbors = new_neighbors;
            office1->neighbors[office1->neighbors_count] = id2;
            office1->neighbors_count += 1;
        }
        exists = 0;
        for (size_t i = 0; i < office2->neighbors_count; i++) {
            if (office2->neighbors[i] == id1) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            unsigned int *new_neighbors = (unsigned int*)realloc(office2->neighbors, (office2->neighbors_count + 1) * sizeof(unsigned int));
            if (!new_neighbors) {
                fclose(file);
                pthread_mutex_unlock(&sys->lock);
                return MEMORY_ERROR;
            }
            office2->neighbors = new_neighbors;
            office2->neighbors[office2->neighbors_count] = id1;
            office2->neighbors_count += 1;
        }
    }
    fclose(file);
    pthread_mutex_unlock(&sys->lock);
    return SUCCESS;
}
