#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int VALID = 0;
const int CORRUPT = 1;
const int INCOMPLETE = 2;

long corrupt_score(char ch)
{
    if (ch == ']') {
        return 57;
    } else if (ch == '}') {
        return 1197;
    } else if (ch == ')') {
        return 3;
    } else if (ch == '>') {
        return 25137;
    }
    return 0;
}

long incomplete_score_ch(char ch)
{
    if (ch == '(') {
        return 1;
    } else if (ch == '[') {
        return 2;
    } else if (ch == '{') {
        return 3;
    } else if (ch == '<') {
        return 4;
    }
    return 0;
}

long incomplete_score(const char *stack, int n)
{
    long score = 0;

    while (n) {
        char ch = stack[n--];
        score *= 5;
        score += incomplete_score_ch(ch);
    }
    return score;
}

int check_line(const char *line, long *score)
{
    char *stack = malloc(256);
    int n = 0;

    const int len = strlen(line);
    int result = -1;
    for (int i=0; i<len && result == -1; i++) {
        char ch = line[i];

        if (ch == '(' || ch == '[' || ch == '{' || ch == '<') {
            stack[++n] = ch;
        } else if (n && ch == ')') {
            if (stack[n--] != '(') {
                result = CORRUPT;
                *score = corrupt_score(ch);
            }
        } else if (n && ch == ']') {
            if (stack[n--] != '[') {
                result = CORRUPT;
                *score = corrupt_score(ch);
            }
        } else if (n && ch == '}') {
            if (stack[n--] != '{') {
                result = CORRUPT;
                *score = corrupt_score(ch);
            }
        } else if (n && ch == '>') {
            if (stack[n--] != '<') {
                result = CORRUPT;
                *score = corrupt_score(ch);
            }
        } else {
            *score = corrupt_score(ch);
            result = CORRUPT;
        }
    }

    if (result == -1) {
        if (n) {
            *score = incomplete_score(stack, n);
            result = INCOMPLETE;
        } else {
            result = VALID;
        }
    }

    free(stack);
    return result;
}

void task1()
{
    void *file = fopen("input", "r");
    char *buf = malloc(256);

    int corruptScore = 0;

    // incomplete scores are sorted in 'list'
    int n = 0;
    long *list = malloc(sizeof(long) * 200);

    char *line = fgets(buf, 256, file);
    while (line) {
        line[strlen(line) - 1] = 0;
        long score = 0;
        int state = check_line(line, &score);

        if (state == VALID) {
            printf("valid:      %s\n\n", line);
        } else if (state == CORRUPT) {
            printf("corrupt:    %s\n", line);
            printf("  score:    %ld\n\n", score);
            corruptScore += score;
        } else if (state == INCOMPLETE) {
            printf("incomplete: %s\n", line);
            printf("     score: %ld\n\n", score);

            int pos = 0;
            while (pos < n && list[pos] < score) {
                pos++;
            }
            for (int i=n; i > pos; i--) {
                list[i] = list[i-1];
            }
            list[pos] = score;
            n++;
        }

        line = fgets(buf, 256, file);
    }

    printf("-------\n");
    for (int i=0; i<n; i++) {
        printf("- %ld\n", list[i]);
    }

    printf("corrupt score: %d\n", corruptScore);
    printf("incopmlete score: %ld\n", list[n/2]);

    free(buf);
    fclose(file);
}

int main()
{
    task1();
    return 0;
}
