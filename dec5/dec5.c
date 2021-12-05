#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int ALLOW_DIAGONALS = 1;

int min(int a, int b)
{
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

int max(int a, int b)
{
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int abs(int x)
{
    if (x < 0) {
        return -x;
    }

    return x;
}

struct line_t
{
    int x1;
    int y1;
    int x2;
    int y2;
};

struct line_t parse_line(const char *str)
{
    struct line_t line;

    line.x1 = atoi(str);
    str = strchr(str, ',') + 1;
    line.y1 = atoi(str);
    str = strchr(str, '>') + 2;
    line.x2 = atoi(str);
    str = strchr(str, ',') + 1;
    line.y2 = atoi(str);

    return line;
}

int valid_line(struct line_t line)
{
    if (line.x1 == line.x2 || line.y1 == line.y2) {
        return 1;
    }

    if (ALLOW_DIAGONALS && abs(line.x1 - line.x2) == abs(line.y1 - line.y2)) {
        return 1;
    }

    return 0;
}

int delta(int from, int to)
{
    if (from < to) {
        return 1;
    } else if (from > to) {
        return -1;
    } else {
        return 0;
    }
}

int main()
{
    int **map = malloc(sizeof(int*) * 1000);
    for (int i=0; i<1000; i++) {
        map[i] = malloc(sizeof(int) * 1000);
        if (!map[i]) {
            puts("malloc failure");
            return 1;
        }
        memset(map[i], 0, sizeof(int) * 1000);
    }
    void *file = fopen("input", "r");
    char *buf = malloc(30);

    while (fgets(buf, 30, file)) {
        struct line_t line = parse_line(buf);

        if (valid_line(line)) {
            int dx = delta(line.x1, line.x2);
            int dy = delta(line.y1, line.y2);
            int x = line.x1;
            int y = line.y1;

            map[y][x]++;
            do {
                x += dx;
                y += dy;
                map[y][x]++;
            } while (x != line.x2 || y != line.y2);
        }
    }

    for (int i=0; i<10; i++) {
        for (int j=0; j<10; j++) {
            if (map[i][j]) {
                printf("%d", map[i][j]);
            } else {
                printf(".");
            }
        }
        printf("\n");
    }

    int count = 0;
    for (int i=0; i<1000; i++) {
        for (int j=0; j<1000; j++) {
            count += (map[i][j] >= 2);
        }
    }
    printf("overlaps: %d\n", count);

    fclose(file);
    free(buf);
    for (int i=0; i<1000; i++) {
        free(map[i]);
    }
    free(map);
    return 0;
}
