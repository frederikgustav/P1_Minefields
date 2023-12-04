#include <stdlib.h>
#include <stdio.h>
#include "minefields.h"

/**
 * Gives an empty minefield
 * @param width the width of the empty minefield
 * @param height the height of the empty minefield
 * @param metric_square_length the size of the squares in the metric
 * @return the function returns a struct representing an empty minefield with the specified
 * dimensions where all squares initially have no mines.
 */
minefield get_empty_minefield(int width, int height) {
    minefield field;

    field.height = height;
    field.width = width;
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
 * @param mine_amount the amount of mines in the minefields
 * @return the function returns a minefield with randomly placed mines based on the specified dimensions,
 * square length, and the number of mines to be placed.
 */
minefield get_random_minefield(int width, int height, int mine_amount) {
    if (mine_amount > width * height) {
        printf("Error: mine_amount is greater than the amount of squares in the minefield");
        exit(EXIT_FAILURE);
    }

    minefield field = get_empty_minefield(width, height);
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
 * Deallocates memory for a minefields matrix
 * @param field the minefield for which to deallocate memory
 */
void free_minefield(minefield field) {
    for (int y = 0; y < field.height; ++y) {
        free(field.matrix[y]);
    }
    free(field.matrix);
}

/**
 * Prints a minefield, example:
 * | O O X |
 * | X O 0 |
 * | O O X |
 * @param field this parameter is used to access information about the minefield and print its contents
 */
void print_minefield(minefield field) {
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
 * Prints a minefield zone, example:
 * | O O X |
 * | X - - |
 * | O - - |
 * @param field the minefield to print
 * @param zone the zone to print
 */
void print_minefield_zone(minefield field, zone zone) {
    for (int y = 0; y < field.height; ++y) {
        printf("| ");

        for (int x = 0; x < field.width; ++x) {
            if (y >= zone.start.y && y <= zone.end.y &&
                x >= zone.start.x && x <= zone.end.x) {
                printf("-  ");
            } else if (field.matrix[y][x].mine == 0) {
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
 * Gives sum of mines in a minefield
 * @param field minefield to check
 * @return amount of mines
 */
int get_minefield_sum(minefield field) {
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

/**
 * Gives the sum of a specific zone in a minefield
 * @param field the overall minefield
 * @param zone the zone to get the sum of
 * @return the sum of the zone
 */
int get_zone_mine_sum(minefield field, zone zone) {
    int mine_sum = 0;
    for (int y = zone.start.y; y <= zone.end.y; ++y) {
        for (int x = zone.start.x; x <= zone.end.x; ++x) {
            if (field.matrix[y][x].mine == 1) {
                mine_sum++;
            }
        }
    }
    return mine_sum;
}

/**
 * Gives the area of a specific zone
 * @param zone the zone to get the area of
 * @return the area of the zone
 */
int get_zone_area(zone zone) {
    return (zone.end.x - zone.start.x) * (zone.end.y - zone.start.y);
}
