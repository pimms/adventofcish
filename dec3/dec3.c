#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int binaryStringToInt(const char *str)
{
    const int len = strlen(str) - 1;
    int result = 0;

    for (int i=0; i<len; i++) {
        if (str[i] == '1') {
            const int bitIndex = len - 1 - i;
            result |= (1 << bitIndex);
        }
    }

    return result;
}

// -- task 1 --

void task1()
{
    void *file = fopen("input", "r");
    char *buf = malloc(16);

    fgets(buf, 16, file);
    int numLines = 0;
    const int width = strlen(buf) - 1;

    int *bitCounts = malloc(sizeof(int) * width);
    for (int i=0; i<width; i++) {
        bitCounts[i] = 0;
    }

    do {
        numLines++;
        const int input = binaryStringToInt(buf);

        for (int i=0; i<width; i++) {
            if (input & (1 << i)) {
                bitCounts[i]++;
            }
        }
    } while (fgets(buf, 16, file));

    // Calculate gamma
    int gamma = 0;
    for (int i=0; i<width; i++) {
        if (bitCounts[i] > numLines / 2) {
            gamma |= (1 << i);
        }
    }

    // Negate to find epsilon
    int mask = 0;
    for (int i=0; i<width; i++) {
        mask |= (1 << i);
    }
    const int epsilon = (~gamma) & mask;

    printf("t1:%d\n", gamma * epsilon);

    free(bitCounts);
    free(buf);
    fclose(file);
}

// -- task 2 --

char *gbuf;

int inputWidth(const char *filename)
{
    void *file = fopen(filename, "r");
    fgets(gbuf, 16, file);
    fclose(file);
    return strlen(gbuf) - 1;
}

int mostCommonBit(const char *filename, int columnIndex, int mask, int *isDone)
{
    void *file = fopen(filename, "r");
    int numLines = 0;
    int numOnes = 0;

    while (fgets(gbuf, 16, file)) {
        int include = 1;
        for (int i=0; i<columnIndex; i++) {
            char ch = gbuf[i] - '0';
            if ((mask & (1 << i)) ^ (ch << i)) {
                include = 0;
            }
        }

        if (include) {
            numOnes += (gbuf[columnIndex] == '1');
            numLines++;
        }
    }

    fclose(file);

    *isDone = (numOnes == numLines || (numOnes == 1 && numLines == 2));

    if (numOnes >= numLines - numOnes) {
        return 1;
    } else {
        return 0;
    }
}

int firstMatch(const char *filename, int mask, int maskWidth)
{
    void *file = fopen(filename, "r");
    while (fgets(gbuf, 16, file)) {
        int include = 1;
        for (int i=0; i<maskWidth; i++) {
            char ch = gbuf[i] - '0';
            if ((mask & (1 << i)) ^ (ch << i)) {
                include = 0;
            }
        }

        if (include) {
            fclose(file);
            return binaryStringToInt(gbuf);
        }
    }

    fclose(file);
    return -1;
}

void task2()
{
    const char *filename = "input";
    gbuf = malloc(16);
    const int width = inputWidth(filename);
    int oxygenMask = 0;
    int co2Mask = 0;

    int oxygenRating = 0;
    for (int bit=0; bit<width && !oxygenRating; bit++) {
        int isDone = 0;
        int mostCommon = mostCommonBit(filename, bit, oxygenMask, &isDone);
        oxygenMask |= (mostCommon << bit);

        if (isDone) {
            oxygenRating = firstMatch(filename, oxygenMask, bit + 1);
        }
    }

    int co2Rating = 0;
    for (int bit=0; bit<width && !co2Rating; bit++) {
        int isDone = 0;
        int mostCommon = mostCommonBit(filename, bit, co2Mask, &isDone);
        co2Mask |= (!mostCommon << bit);

        if (isDone) {
            co2Rating = firstMatch(filename, co2Mask, bit + 1);
        }
    }

    printf("t2:%d\n", co2Rating * oxygenRating);

    free(gbuf);
}

int main()
{
    task1();
    task2();
    return 0;
}
