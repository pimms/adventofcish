#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void task1(const char *filepath)
{
    void *file = fopen(filepath, "r");
    if (!file) return;

    char *buf = malloc(32);

    int ver = 0;
    int hor = 0;

    while (fgets(buf, 32, file)) {
        int vd = 0;
        int hd = 0;

        if (strstr(buf, "forward")) hd = 1;
        if (strstr(buf, "up"))      vd = -1;
        if (strstr(buf, "down"))    vd = 1;

        char *numstr = strstr(buf, " ");
        if (!++numstr) return;

        const int n = atoi(numstr);
        ver += vd * n;
        hor += hd * n;
    }

    printf("Final position: (%d, %d)\n", hor, ver);
    printf("Multiplied: %d\n", hor * ver);

    free(buf);
    fclose(file);
}

void task2(const char *filepath)
{
    void *file = fopen(filepath, "r");
    if (!file) return;

    char *buf = malloc(32);

    int aim = 0;
    int ver = 0;
    int hor = 0;

    while (fgets(buf, 32, file)) {
        int vd = 0;
        int hd = 0;
        int ad = 0;

        if (strstr(buf, "forward")) { hd = 1; vd = aim; }
        if (strstr(buf, "up"))      { ad = -1; }
        if (strstr(buf, "down"))    { ad = 1; }

        char *numstr = strstr(buf, " ");
        if (!++numstr) return;

        const int n = atoi(numstr);
        aim += ad * n;
        ver += vd * n;
        hor += hd * n;
    }

    printf("Final position: (%d, %d)\n", hor, ver);
    printf("Multiplied: %d\n", hor * ver);

    free(buf);
    fclose(file);
}

int main(int argc, char **argv)
{
    printf("argc: %d\n", argc);
    char *filepath = malloc(20);
    if (argc > 1) {
        strcpy(filepath, argv[1]);
    } else {
        strcpy(filepath, "input");
    }

    printf("-- task 1 --\n");
    task1(filepath);
    printf("-- task 2 --\n");
    task2(filepath);
    return 0;
}
