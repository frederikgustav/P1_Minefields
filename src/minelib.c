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
    field.matrix = malloc(sizeof(struct square **) * height);

    for (int i = 0; i < height; ++i) {
        field.matrix[i] = malloc(sizeof(struct square *) * width);
        for (int j = 0; j < width; ++j) {
            field.matrix[i][j].mine = 0;
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
    for (int i = 0; i < field.height; ++i) {
        printf("| ");

        for (int j = 0; j < field.width; ++j) {
            if (field.matrix[i][j].mine == 0) {
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

    for (int i = 0; i < field.height; ++i) {
        free(field.matrix[i]);
    }
    free(field.matrix);
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

struct sub_minefield get_biggest_cleared_sub_minefield(struct minefield field) {
    int max_size =0;
    int max_i = 0, max_j = 0;
    int dp[field.height][field.width];

    for (int i = 0; i < field.height; ++i) {
        for (int j =0; j < field.width; ++j) {
            dp[i][j] = (field.matrix[i][j].mine == 0) ? 1 : 0;
            if (dp[i][j] >max_size) {
                max_size =dp[i][j];
                max_i;
                max_j;
            }
        }
    }

    for (int i = 1; i < field.height; ++i) {
        for (int j = 1; j < field.width; ++j) {
            if (field.matrix[i][j].mine == 0) {
                dp[i][j] = 1 + min(min(dp[i - 1][j - 1], dp[i - 1][j]), dp[i][j - 1]);
                if (dp[i][j] > max_size) {
                    max_size = dp[i][j];
                    max_i = i;
                    max_j = j;
                }
            }
        }
    }

    struct sub_minefield biggest_square;
    biggest_square.start_point.x = max_j - max_size + 1;
    biggest_square.start_point.y = max_i - max_size + 1;
    biggest_square.end_point.x = max_j;
    biggest_square.end_point.y = max_i;

    return biggest_square;
}
