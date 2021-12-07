#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
==================
      NAIVE
==================
*/
struct node_t
{
    struct node_t *next;
    int val;
};

struct list_t
{
    struct node_t *head;
    struct node_t *tail;
    int count;
};

struct list_t *naive_read(const char *filename)
{
    struct list_t *list = malloc(sizeof(struct list_t));
    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    void *file = fopen(filename, "r");
    char *buf = malloc(1024);
    fgets(buf, 1024, file);

    char *str = buf;

    do {
        struct node_t *node = malloc(sizeof(struct node_t));
        node = malloc(sizeof(struct node_t));
        node->val = atoi(str);
        node->next = NULL;

        if (!list->head) {
            list->head = node;
            list->tail = node;
        }

        list->tail->next = node;
        list->tail = node;
        list->count++;

        str = strchr(str, ',');
        if (str) {
            str++;
        }
    } while (str);

    free(buf);
    fclose(file);

    return list;
}

void naive_spawn(struct list_t *list)
{
    struct node_t *node = malloc(sizeof(struct node_t));
    node->val = 8;
    list->tail->next = node;
    list->tail = node;
    list->count++;
}

void naive_tick(struct list_t *list)
{
    struct node_t *node = list->head;
    int spawns = 0;

    while (node) {
        if (node->val == 0) {
            node->val = 6;
            spawns++;
        } else {
            node->val--;
        }

        node = node->next;
    }

    for (int i=0; i<spawns; i++) {
        naive_spawn(list);
    }
}

void naive_print(struct list_t *list)
{
    struct node_t *node = list->head;
    int spawns = 0;

    while (node) {
        printf("%d ", node->val);
        node = node->next;
    }
    printf("\n");
}

void naive_solution(const char *filename, int ticks)
{
    struct list_t *list = naive_read("input");

    for (int i=0; i<ticks; i++) {
        naive_tick(list);
    }
    printf("%d fish after %d days\n", list->count, ticks);
}

/*
==================
     PROPER
==================
*/
long* proper_read(const char *filename)
{
    long *result = malloc(sizeof(long) * 9);
    memset(result, 0, sizeof(long) * 9);

    void *file = fopen(filename, "r");
    char *buf = malloc(1024);
    fgets(buf, 1024, file);

    char *str = buf;

    do {
        long val = atoi(str);
        result[val]++;
        str = strchr(str, ',');
        if (str) {
            str++;
        }
    } while (str);

    free(buf);
    fclose(file);
    return result;
}

void proper_tick(long *list)
{
    const long spawns = list[0];
    for (int i=0; i<8; i++) {
        list[i] = list[i+1];
    }
    list[6] += spawns;
    list[8] = spawns;
}

void proper_solution(const char *filename, int ticks)
{
    long *list = proper_read(filename);
    for (int i=0; i<ticks; i++) {
        proper_tick(list);
    }

    long sum = 0;
    for (int i=0; i<9; i++) {
        printf("%d: %ld\n", i, list[i]);
        sum += list[i];
    }
    printf("%ld fish after %d days\n", sum, ticks);
    free(list);
}

int main(int argc, char **argv)
{
    if (argc != 4) {
        printf("missing params: (naive|proper) <filename> <ticks>\n");
        return 1;
    }

    const char *method = argv[1];
    const char *filename = argv[2];
    const int ticks = atoi(argv[3]);

    printf("%s method of file '%s' with %d ticks\n", method, filename, ticks);

    if (strcmp("naive", method) == 0) {
        naive_solution(filename, ticks);
    } else {
        proper_solution(filename, ticks);
    }

    return 0;
}
