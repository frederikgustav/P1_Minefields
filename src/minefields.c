#include <stdlib.h>
#include <stdio.h>
#include "minefields.h"

/**
 * Gives an empty minefield
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
 * Gives a minefield with randomly placed mines
 * @param width the width of the empty minefield with mines in it
 * @param height the height of the empty minefield with mines in it
 * @param metric_square_length the size of the squares in the metric
 * @param mine_amount the amount of mines in the minefields
 * @return the function returns a minefield with randomly placed mines based on the specified dimensions,
 * square length, and the number of mines to be placed.
 */
struct minefield get_random_minefield(int width, int height, double metric_square_length, int mine_amount) {
    struct minefield field = get_empty_minefield(width, height, metric_square_length);

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

struct minefield get_minefield_copy(struct minefield field) {
    struct minefield field_copy = get_empty_minefield(field.width, field.height, field.metric_square_length);

    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            field_copy.matrix[y][x].mine = field.matrix[y][x].mine;
        }
    }

    return field_copy;
}

/**
 * Prints a minefield, example:
 * | O O X |
 * | X O 0 |
 * | O O X |
 * @param field this parameter is used to access information about the minefield and print its contents
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
    printf("\n");
}

/**
 * Deallocates memory for a minefields matrix
 * @param field the minefield for which to deallocate memory
 */
void free_minefield(struct minefield field) {
    for (int y = 0; y < field.height; ++y) {
        free(field.matrix[y]);
    }
    free(field.matrix);
}

/**
 * Gives sum of mines in a minefield
 * @param field minefield to check
 * @return amount of mines
 */
int get_minefield_sum(struct minefield field) {
    int sum = 0;
    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            if (field.matrix[y][x].mine) {
                sum++;
            }
        }
    }
    return sum;
}
