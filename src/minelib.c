#include "minelib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct minefield get_empty_minefield(int width, int height, double metric_square_length) {
    /*
     * Takes a width, height and metric_square_length, referring to the metric length of
     * a square, and returns an empty minefield.
     */
    struct minefield field;

    field.height = height;
    field.width = width;
    field.metric_square_length = metric_square_length;
    field.matrix = malloc(sizeof(struct square *) * height);

    for (int y = 0; y < height; ++y) {
        field.matrix[y] = malloc(sizeof(struct square) * width);
        for (int x = 0; x < width; ++x) {
            field.matrix[y][x].mine = 0;
        }
    }

    return field;
}

struct minefield get_random_minefield(int width, int height, double metric_square_length, int mine_amount) {
    /*
     * Takes a width, height, mine_amount and metric_square_length, referring to the metric length of
     * a square, and returns a minefield with randomly placed mines.
     */
    struct minefield field = get_empty_minefield(width, height, metric_square_length);
    srand(time(NULL));

    // For-loop generating random mines
    for (int i = 0; i < mine_amount; ++i) {
        int random_y = rand() % height;
        int random_x = rand() % width;

        if (field.matrix[random_y][random_x].mine) {
            i--;
            continue;
        }

        field.matrix[random_y][random_x].mine = 1;
    }

    return field;
}

void print_minefield(struct minefield field) {
    /*
     * Prints a minefield, example:
     * | O O X |
     * | X O 0 |
     * | O O X |
     */
    for (int y = 0; y < field.height; ++y) {
        printf("| ");

        for (int x = 0; x < field.width; ++x) {
            if (field.matrix[y][x].mine == 0) {
                printf("0  ");
            } else {
                printf("X  ");
            }
        }

        printf("| \n");
    }
}

void free_minefield(struct minefield field) {
    /*
     * Takes a minefield, and deallocates the memory.
     */
    for (int y = 0; y < field.height; ++y) {
        free(field.matrix[y]);
    }
    free(field.matrix);
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

struct sub_minefield get_biggest_cleared_sub_minefield(struct minefield field) {
    int max_size = 0;
    int max_y = 0, max_x = 0;
    int dp[field.height][field.width];

    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            dp[y][x] = (field.matrix[y][x].mine == 0) ? 1 : 0;
            if (dp[y][x] >max_size) {
                max_size =dp[y][x];
                max_y = y;
                max_x = x;
            }
        }
    }

    for (int y = 1; y < field.height; ++y) {
        for (int x = 1; x < field.width; ++x) {
            if (field.matrix[y][x].mine == 0) {
                dp[y][x] = 1 + min(min(dp[y - 1][x - 1], dp[y - 1][x]), dp[y][x - 1]);
                if (dp[y][x] > max_size) {
                    max_size = dp[y][x];
                    max_y = y;
                    max_x = x;
                }
            }
        }
    }

    struct sub_minefield biggest_square;
    biggest_square.start_point.x = max_x - max_size + 1;
    biggest_square.start_point.y = max_y - max_size + 1;
    biggest_square.end_point.x = max_x;
    biggest_square.end_point.y = max_y;

    return biggest_square;
}

