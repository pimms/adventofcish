#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*  ======= */
/* STRUCTS */
/* =====  */

struct edge
{
    // Forward declaration of structs is not supported in Cish.
    // This will have to do :)
    void *vert;

    struct edge *next;
};

struct vertex
{
    char *name;
    int visits;
    int maxVisits;
    struct edge *edges;
};

struct vnode
{
    struct vertex *vert;
    struct vnode *next;
};

void vertex_free(struct vertex *v);

/*  ==== */
/* EDGE */
/* ===  */

struct edge* edge_make(struct vertex *vert)
{
    struct edge *edge = malloc(sizeof(struct edge));
    memset(edge, 0, sizeof(struct vnode));
    edge->vert = vert;
    return edge;
}

void edge_free(struct edge *edge)
{
    if (edge->next) {
        edge_free(edge->next);
    }

    free(edge);
}

/*  ===== */
/* VNODE */
/* ===  */

struct vnode* vnode_make(struct vertex *vert)
{
    struct vnode *node = malloc(sizeof(struct vnode));
    memset(node, 0, sizeof(struct vnode));
    node->vert = vert;
    return node;
}

void vnode_free(struct vnode *node)
{
    if (node->next) {
        vnode_free(node->next);
    }

    vertex_free(node->vert);
    free(node);
}

struct vnode* vnode_find_by_name(struct vnode *list, const char *name)
{
    while (list && strcmp(list->vert->name, name)) {
        list = list->next;
    }

    return list;
}

void vnode_vertex_connect(struct vnode *a, struct vnode *b)
{
    struct edge *acon = edge_make(b->vert);
    acon->next = a->vert->edges;
    a->vert->edges = acon;

    struct edge *bcon = edge_make(a->vert);
    bcon->next = b->vert->edges;
    b->vert->edges = bcon;
}

struct vnode* vnode_prepend(struct vnode *list, struct vnode *node)
{
    node->next = list;
    return node;
}

/*  ====== */
/* VERTEX */
/* ====  */

struct vertex* vertex_make(const char *name)
{
    struct vertex *vert = malloc(sizeof(struct vertex));
    memset(vert, 0, sizeof(struct vertex));
    if (name[0] >= 'A' && name[0] <= 'Z') {
        vert->maxVisits = 0x7FFFFFFF;
    } else {
        vert->maxVisits = 1;
    }

    vert->name = malloc(strlen(name) + 1);
    strcpy(vert->name, name);
    return vert;
}

void vertex_free(struct vertex *v)
{
    if (v->edges) {
        edge_free(v->edges);
    }

    free(v->name);
    free(v);
}

/*  ===== */
/* GRAPH */
/* ===  */

struct vnode* graph_read(const char *filename)
{
    struct vnode *root = NULL;
    void *file = fopen(filename, "r");
    char *buf = malloc(40);
    char *line = fgets(buf, 40, file);

    while (line) {
        // Ignore trailing newline
        line[strlen(line)-1] = 0;

        char *name1 = line;
        char *name2 = strchr(line, '-');
        *name2 = 0;
        name2++;

        struct vnode *va;
        va = vnode_find_by_name(root, name1);
        if (!va) {
            va = vnode_make(vertex_make(name1));

            if (root) {
                root->next = vnode_prepend(root->next, va);
            } else {
                root = va;
            }
        }

        struct vnode *vb;
        vb = vnode_find_by_name(root, name2);
        if (!vb) {
            vb = vnode_make(vertex_make(name2));
            root->next = vnode_prepend(root->next, vb);
        }

        vnode_vertex_connect(va, vb);

        line = fgets(buf, 40, file);
    }

    fclose(file);
    return root;
}

/* ====== TASK 1 ====== */

void spaces(int n)
{
    for (int i=0; i<n; i++) {
        printf(" ");
    }
}

int is_regular_small_cave(struct vertex *vert)
{
    if (!strcmp("start", vert->name)) {
        return 0;
    } else if (vert->name[0] >= 'a' && vert->name[0] <= 'z') {
        return 1;
    }
    return 0;
}

int recursive_paths(struct vertex *vert, int twiceToken, int n)
{
    if (strcmp(vert->name, "end") == 0) {
        // printf("v: ");
        // spaces(n);
        // printf("end!\n");
        return 1;
    }
    int sum = 0;

    // printf("v: ");
    // spaces(n);
    // printf("%s\n", vert->name);

    if (vert->visits >= vert->maxVisits) {
        if (twiceToken && is_regular_small_cave(vert)) {
            twiceToken = 0;
        } else {
            return 0;
        }
    }

    vert->visits++;

    struct edge *edge = vert->edges;
    while (edge) {
        struct vertex *other = (struct vertex*)edge->vert;
        sum += recursive_paths(other, twiceToken, n+1);
        edge = edge->next;
    }

    vert->visits--;

    return sum;
}

void task1()
{
    struct vnode *root = graph_read("sample1");
    struct vnode *start = vnode_find_by_name(root, "start");
    const int paths = recursive_paths(start->vert, 1, 0);
    printf("paths: %d\n", paths);
}

void task2()
{
    struct vnode *root = graph_read("input");
    struct vnode *start = vnode_find_by_name(root, "start");
    const int paths = recursive_paths(start->vert, 1, 0);
    printf("paths: %d\n", paths);
}

int main()
{
    task2();
    return 0;
}
