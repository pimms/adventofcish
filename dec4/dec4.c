#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
==================
input_t
==================
*/

struct input_t
{
    int val;
    struct input_t *next;
};

struct input_t* read_input(void *file)
{
    char *buf = malloc(512);
    if (!fgets(buf, 512, file)) {
        printf("Failed to read input");
        free(buf);
        return NULL;
    }

    struct input_t *head = NULL;
    struct input_t *tail = NULL;

    char *str = buf;
    while (str) {
        struct input_t *current = malloc(sizeof(struct input_t));
        current->val = atoi(str);
        current->next = NULL;

        // If we don't have a head, this is the first input.
        if (!head) head = current;

        // If we have a tail, add current to the list.
        if (tail) tail->next = current;

        // Whatever just happened, current is the tail.
        tail = current;

        str = strchr(str, ',');
        if (str) {
            // If we found a comma, hop to the next byte.
            str++;
        }
    }

    // Discard the empty line.
    fgets(buf, 512, file);

    free(buf);
    return head;
}

void free_input(struct input_t *input)
{
    struct input_t *next;

    while (input) {
        next = input->next;
        free(input);
        input = next;
    }
}

/*
==================
bingo_t
==================
*/

struct bingo_t
{
    int **board;
    struct bingo_t *prev;
    struct bingo_t *next;
};

struct bingo_t* make_bingo()
{
    struct bingo_t *bingo = malloc(sizeof(struct bingo_t));
    bingo->prev = NULL;
    bingo->next = NULL;
    bingo->board = malloc(sizeof(int*) * 5);
    for (int i=0; i<5; i++) {
        bingo->board[i] = malloc(sizeof(int) * 5);
    }

    return bingo;
}

void free_bingo(struct bingo_t *bingo)
{
    for (int i=0; i<5; i++) {
        free(bingo->board[i]);
    }

    free(bingo->board);
    free(bingo);
}

struct bingo_t* read_bingo(void *file, int *hasMore)
{
    char *buf = malloc(30);
    struct bingo_t *bingo = make_bingo();

    for (int i=0; i<5; i++) {
        if (!fgets(buf, 30, file)) {
            printf("ERROR: Unexpected EOF");
            return NULL;
        }

        // sscanf would have been nice about now... :)
        // Don't have the time to implement in cish, so
        // here we go... :)
        char *str = buf;
        for (int j=0; j<5; j++) {
            // The numbers are space-separated, but they
            // sometimes contains more than one space, so
            // we got to deal with that :)
            while (str && *str == ' ') {
                str++;
            }

            bingo->board[i][j] = atoi(str);
            str = strchr(str, ' ');
        }
    }

    // Attempt to read the empty line. If this call succeeds,
    // there are more boards to be read.
    *hasMore = (fgets(buf, 30, file) != NULL);

    free(buf);
    return bingo;
}

struct bingo_t *read_all_bingos(void *file)
{
    int hasMore = 1;
    struct bingo_t *head = read_bingo(file, &hasMore);
    if (!head) {
        return NULL;
    }

    struct bingo_t *tail = head;
    while (hasMore) {
        struct bingo_t *next = read_bingo(file, &hasMore);

        tail->next = next;
        next->prev = tail;

        tail = next;
    }

    return head;
}

void mark_board(struct bingo_t *bingo, int num)
{
    for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++) {
            if (bingo->board[i][j] == num) {
                bingo->board[i][j] = -1;
            }
        }
    }
}

int has_bingo(struct bingo_t *bingo)
{
    for (int i=0; i<5; i++) {
        int row = 1;
        int col = 1;
        for (int j=0; j<5; j++) {
            if (bingo->board[i][j] != -1) {
                row = 0;
            }
            if (bingo->board[j][i] != -1) {
                col = 0;
            }
        }
        if (row || col ) return 1;
    }

    return 0;

    /*
     * Oops, diagonals don't count
    int diag1 = 1;
    int diag2 = 1;
    for (int i=0; i<5; i++) {
        if (bingo->board[i][i] != -1) {
            diag1 = 0;
        }
        if (bingo->board[4-i][i] != -1) {
            diag2 = 0;
        }
    }

    return diag1 || diag2;
    */
}

void print_bingo(struct bingo_t *bingo)
{
    for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++) {
            printf("%d ", bingo->board[i][j]);
        }
        printf("\n");
    }
    printf("has bingo: %d\n\n", has_bingo(bingo));
}

int unmarked_sum(struct bingo_t *bingo)
{
    int sum = 0;
    for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++) {
            int val = bingo->board[i][j];
            if (val != -1) {
                sum += val;
            }
        }
    }

    return sum;
}

int main()
{
    void *file = fopen("input", "r");
    struct input_t *inputRoot= read_input(file);
    struct bingo_t *bingoRoot = read_all_bingos(file);

    int winners = 0;

    struct input_t *input = inputRoot;
    while (input) {
        printf("Drawing value: %d\n", input->val);

        struct bingo_t *bingo = bingoRoot;
        while (bingo) {
            mark_board(bingo, input->val);
            if (has_bingo(bingo)) {
                printf("BINGO!\n");
                if (winners++ == 0) {
                    printf("task1 answer: %d\n", input->val * unmarked_sum(bingo));
                }

                if (!bingo->prev && !bingo->next) {
                    printf("task2 answer: %d\n", input->val * unmarked_sum(bingo));
                }

                // Remove current board from list
                if (bingo == bingoRoot) {
                    bingoRoot = bingo->next;
                }

                struct bingo_t *prev = bingo->prev;
                struct bingo_t *next = bingo->next;
                if (prev) prev->next = next;
                if (next) next->prev = prev;
                free_bingo(bingo);

                bingo = next;
            } else {
                bingo = bingo->next;
            }
        }

        input = input->next;
    }

    free_input(input);
    fclose(file);
    return 0;
}
