#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

int abs(int n)
{
    if (n < 0)
        return -n;
    return n;
}

struct node_t
{
    struct node_t *next;
    int pos;
    int count;
};

struct list_t
{
    struct node_t *head;
    struct node_t *tail;
    int count;
    int minPt;
    int maxPt;
};

struct node_t *find_pos(struct list_t *list, int pos)
{
    struct node_t *node = list->head;
    while (node) {
        if (node->pos == pos) {
            return node;
        }
        node = node->next;
    }

    return NULL;
}

struct list_t *list_read(const char *filename)
{
    struct list_t *list = malloc(sizeof(struct list_t));
    list->count = 0;
    list->head = NULL;
    list->tail = NULL;
    list->minPt = 0x7FFFFFFF;
    list->maxPt = -1;

    void *file = fopen(filename, "r");
    char *buf = malloc(4096);
    fgets(buf, 4096, file);

    char *str = buf;

    do {
        const int pos = atoi(str);
        struct node_t *foundNode = find_pos(list, pos);
        if (foundNode != NULL) {
            foundNode->count++;
        } else {
            struct node_t *node = malloc(sizeof(struct node_t));
            node = malloc(sizeof(struct node_t));
            node->pos = pos;
            node->count = 1;
            node->next = NULL;

            list->minPt = min(list->minPt, node->pos);
            list->maxPt = max(list->maxPt, node->pos);

            if (!list->head) {
                list->head = node;
                list->tail = node;
            } else {
                list->tail->next = node;
            }

            list->tail = node;
            list->count++;
        }

        str = strchr(str, ',');
        if (str) {
            str++;
        }
    } while (str);

    free(buf);
    fclose(file);

    return list;
}

int alignment_cost(struct list_t *list, int pos)
{
    int sum = 0;

    struct node_t *node = list->head;
    while (node) {
        sum += abs(node->pos - pos) * node->count;
        node = node->next;
    }

    return sum;
}

int alignment_cost2(struct list_t *list, int pos)
{
    int sum = 0;

    struct node_t *node = list->head;
    while (node) {
        const int dist = abs(node->pos - pos);
        const int fuel = (dist * (dist + 1)) / 2;
        sum += fuel * node->count;
        node = node->next;
    }

    return sum;
}

void task1()
{
    struct list_t *list = list_read("input");
    int lowestCost = 0x7FFFFFFF;
    int lowestPos = list->minPt;

    struct node_t *node = list->head;
    while (node) {
        printf("pos %d: %d crabs\n", node->pos, node->count);
        node = node->next;
    }

    const int listMin = list->minPt;
    const int listMax = list->maxPt;
    printf("lowpt: %d, hipt: %d\n", listMin, listMax);

    for (int i=listMin; i<listMax; i++) {
        const int cost = alignment_cost2(list, i);
        printf("align %d: %d fuel\n", i, cost);
        if (cost < lowestCost) {
            lowestCost = cost;
            lowestPos = i;
        }
    }

    printf("best point: %d\n", lowestPos);
    printf("move cost:  %d\n", lowestCost);
}

int main()
{
    task1();
    return 0;
}
