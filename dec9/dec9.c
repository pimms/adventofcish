#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *RED = "\033[0;31m";
const char *WHITE = "\033[0m";

struct map
{
    int width;
    int height;
    int **map;
};

struct map *read_map(const char *filename)
{
    void *file = fopen(filename, "r");
    char *buf = malloc(200);

    // Find the dimensions of the map
    int height = 0;
    while(fgets(buf, 200, file)) { height++; }
    fclose(file);

    file = fopen(filename, "r");
    struct map *map = malloc(sizeof(struct map));
    memset(map, 0, sizeof(struct map));

    const char *line = fgets(buf, 200, file);
    const int width = strlen(line) - 1;

    map->width = width;
    map->height = height;
    map->map = malloc(sizeof(int*) * height);

    int row = 0;
    while (line) {
        map->map[row] = malloc(sizeof(int) * map->width);

        for (int i=0; i<width; i++) {
            map->map[row][i] = line[i] - '0';
        }

        line = fgets(buf, 200, file);
        row++;
    }

    free(buf);
    fclose(file);
    return map;
}

void task1()
{
    struct map *map = read_map("input");

    // Cish doesn't support struct-access in for-loops... :)
    const int width = map->width;
    const int height = map->height;
    int result = 0;

    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            const int val = map->map[y][x];
            int isLow = 1;

            if (x && map->map[y][x-1] <= val)
                isLow = 0;
            if (x+1 < width && map->map[y][x+1] <= val)
                isLow = 0;
            if (y && map->map[y-1][x] <= val)
                isLow = 0;
            if (y+1 < height && map->map[y+1][x] <= val)
                isLow = 0;

            if (isLow) {
                printf("%d", val);
                result += val + 1;
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("task 1: %d\n", result);
}

//

int basin(int x, int y, struct map *map)
{
    if (x < 0 || y < 0 || x >= map->width || y >= map->height) {
        return 0;
    }
    if (map->map[y][x] == 9) {
        return 0;
    }

    map->map[y][x] = 9;
    int sum = 1;
    sum += basin(x-1, y,   map);
    sum += basin(x+1, y,   map);
    sum += basin(x,   y-1, map);
    sum += basin(x,   y+1, map);
    return sum;
}

void task2()
{
    int nbasins = 0;
    int *basins = malloc(sizeof(int) * 1000);
    struct map *map = read_map("input");
    const int width = map->width;
    const int height = map->height;

    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            if (map->map[y][x] != 9) {
                const int size = basin(x, y, map);

                printf("basin size %d\n", size);

                // Shitty insertion sort, ascending
                int pos = 0;
                while (pos < nbasins && basins[pos] < size) {
                    pos++;
                }
                for (int i=nbasins; i >= pos; i--) {
                    basins[i] = basins[i-1];
                }
                basins[pos] = size;
                nbasins++;
            }
        }
    }

    printf("----------\n");
    printf("Basins: %d\n", nbasins);
    for (int i=0; i<nbasins; i++) {
        printf("%d ", basins[i]);
    }
    printf("\n\n");

    const int result = basins[nbasins-1] * basins[nbasins-2] * basins[nbasins-3];
    printf("result: %d\n", result);
}

int main()
{
    task2();
    return 0;
}
