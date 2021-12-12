#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*  ======= */
/* STRUCTS */
/* =====  */

struct vertex
{
    char *name;
    int visited;
    int large;
    struct vertex *con;
};

struct vnode
{
    struct vertex *vert;
    struct vnode *edges;
    struct vnode *next;
};

void vertex_free(struct vertex *v);

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
    struct vnode *acon = vnode_make(b->vert);
    acon->next = a->edges;
    a->edges = acon;

    struct vnode *bcon = vnode_make(a->vert);
    bcon->next = b->edges;
    b->edges = bcon;
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

    if (name[0] > 'A' && name[0] <= 'Z') {
        vert->large = 1;
    }

    vert->name = malloc(strlen(name) + 1);
    strcpy(vert->name, name);
    return vert;
}

void vertex_free(struct vertex *v)
{
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

        printf("%s - %s\n", name1, name2);

        struct vnode *va;
        va = vnode_find_by_name(root, name1);
        if (!va) {
            printf("first ref to '%s'\n", name1);
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
            printf("first ref to '%s'\n", name2);
            vb = vnode_make(vertex_make(name2));
            root->next = vnode_prepend(root->next, vb);
        }

        printf("connecting %s and %s\n", name1, name2);
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

int recursive_paths(struct vnode *node, int n)
{
    spaces(n);
    if (strcmp(node->vert->name, "end") == 0) {
        printf("end!\n");
        return 1;
    }
    printf("%s\n", node->vert->name);

    int sum = 0;

    node->vert->visited = 1;

    struct vnode *edge = node->next;
    while (edge) {
        if (!edge->vert->visited || edge->vert->large) {
            sum += recursive_paths(edge, n+1);
        }

        edge = edge->next;
    }

    node->vert->visited = 0;
    return sum;
}

void task1()
{
    struct vnode *root = graph_read("sample");

    struct vnode *n = root;
    printf("ALL NODES:\n");
    while (n) {
        printf(" - %s\n", n->vert->name);

        struct vnode *e = n->edges;
        while (e) {
            printf("   - %s\n", e->vert->name);
            e = e->next;
        }

        n = n->next;
    }

    const struct vnode *start = vnode_find_by_name(root, "start");
    start->vert->visited = 1;
    const int paths = recursive_paths(start->edges, 0);
    printf("paths: %d\n", paths);
}

int main()
{
    task1();
    return 0;
}
