#include <stdio.h>
#include <stdlib.h>

const int BUFSIZE = 32;

int read_int(void *file, char *buf)
{
    if (!fgets(buf, BUFSIZE, file)) {
        return -1;
    }

    return atoi(buf);
}

void task1()
{
    void *file = fopen("input1", "r");
    if (!file) return;

    char *buf = malloc(BUFSIZE);

    int increases = 0;
    int prev = 0x7FFFFFFF;

    while (fgets(buf, BUFSIZE, file)) {
        int current = atoi(buf);
        if (current > prev) {
            increases++;
        }
        prev = current;
    }


    free(buf);
    printf("task1: %d\n", increases);
}

void task2()
{
    void *file = fopen("input1", "r");
    if (!file) return;

    char *buf = malloc(BUFSIZE);
    int *window = malloc(sizeof(int) * 3);

    window[0] = read_int(file, buf);
    window[1] = read_int(file, buf);
    window[2] = read_int(file, buf);

    int increases = 0;

    int num = read_int(file, buf);
    while (num > 0) {
        const int prev = window[0] + window[1] + window[2];
        const int current = window[1] + window[2] + num;
        window[0] = window[1];
        window[1] = window[2];
        window[2] = num;

        if (current > prev) {
            increases++;
        }

        num = read_int(file, buf);
    }

    free(buf);
    free(window);
    printf("task2: %d\n", increases);
}

int main()
{
    task1();
    task2();
    return 0;
}
