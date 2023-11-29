#include "minelib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/**
 *
 * Takes a width, height and metric_square_length, referring to the metric length of a square,
 * and returns an empty minefield.
 * @param width the width of the empty minefield
 * @param height the height of the empty minefield
 * @param metric_square_length the size of the squares in the metric
 * @return the function returns a struct representing an empty minefield with the specified
 * dimensions and square length, where all squares initially have no mines.
 */
struct minefield get_empty_minefield(int width, int height, double metric_square_length) {
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

/**
 * Takes a width, height, mine_amount and metric_square_length, referring to the metric length of
 * a square, and returns a minefield with randomly placed mines.
 * @param width the width of the empty minefield with mines in it
 * @param height the height of the empty minefield with mines in it
 * @param metric_square_length the size of the squares in the metric
 * @param mine_amount the amount of mines in the minefields
 * @return the function returns a minefield with randomly placed mines based on the specified dimensions,
 * square length, and the number of mines to be placed.
 */
struct minefield get_random_minefield(int width, int height, double metric_square_length, int mine_amount) {
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

/**
 * Prints a minefield, example:
 * | O O X |
 * | X O 0 |
 * | O O X |
 * @param field this parameter is used to access information about the minefield and print its contents
 * it prints the minefield as 0 and xs
 */
void print_minefield(struct minefield field) {
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

/**
 *
 * @param field Takes a minefield, and deallocates the memory
 */
void free_minefield(struct minefield field) {
    for (int y = 0; y < field.height; ++y) {
        free(field.matrix[y]);
    }
    free(field.matrix);
}

/**
 *
 * @param a
 * @param b
 * @return it returns the minimum of a and b. If a is less than b, it returns a; otherwise, it returns b.
 */
int min(int a, int b) {
    return (a < b) ? a : b;
}

/**
 *
 * @param field the parameter field is used to provide information about the generated minefield for this function
 * to find the largest possible area within the minefield that can be cleared (that has as many mines as possible)
 * @return it retures the area that contains the most mines
 */
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

