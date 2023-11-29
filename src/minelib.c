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
    printf("\n");
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

/**
 * Gives factorial of number
 * @param n start number
 * @return
 */
int factorial(int n) {
    int result = n;
    for (int i = n-1; i >= 1; --i) {
        result *= i;
    }
    return result;
}

/**
 * Gives biggest clearable area in a minefield
 * @param field
 * @param mine_capacity
 * @return
 */
struct sub_minefield get_biggest_clearable_sub_minefield(struct minefield field, int mine_capacity) {
    int mine_sum = get_minefield_sum(field);
    int permutation_amount = factorial(mine_sum)/(factorial(mine_capacity)*factorial(mine_sum-mine_capacity));
    struct sub_minefield cleared_area = {
            .start_point = {0, 0},
            .end_point = {0, 0},
    };

    // Create permutations array
    int** permutations = malloc(sizeof(int*) * permutation_amount);
    for (int i = 0; i < permutation_amount; ++i) {
        permutations[i] = NULL;
    }

    // Create seed permutation
    int* permutation = malloc(sizeof(int) * mine_sum);
    for (int mine = 0; mine < mine_sum; ++mine) {
        permutation[mine] = -1;
    }

    generate_permutations(mine_sum-mine_capacity, mine_sum, permutation, permutations);

    // Map each permutation to the original minefield, and keep the biggest cleared_area
    int current_mine = 0;
    int current_clearence = 0;
    int this_clearence = 0;
    for (int current_permutation = 0; current_permutation < permutation_amount; ++current_permutation) {
        // Map permutation to minefield
        struct minefield current_field = get_empty_minefield(field.width, field.height, field.metric_square_length);
        for (int y = 0; y < field.height; ++y) {
            for (int x = 0; x < field.width; ++x) {
                if (field.matrix[y][x].mine == 1) {
                    current_field.matrix[y][x].mine = permutations[current_permutation][current_mine];
                    current_mine++;
                }
            }
        }
        current_mine = 0;

        // Get the biggest cleared area
        struct sub_minefield biggest_cleared_area = get_biggest_cleared_sub_minefield(current_field);
        current_clearence = (biggest_cleared_area.end_point.x - biggest_cleared_area.start_point.x + 1) *
                            (biggest_cleared_area.end_point.y - biggest_cleared_area.start_point.y + 1);
        this_clearence = (cleared_area.end_point.x - cleared_area.start_point.x + 1) *
                         (cleared_area.end_point.y - cleared_area.start_point.y + 1);

        // If current cleared area is bigger than the previous biggest cleared area, set it as the new biggest cleared area
        if (current_clearence > this_clearence) {
            cleared_area = biggest_cleared_area;
        }

        // print minefield
        printf("Permutation %d:\n", current_permutation);
        print_minefield(current_field);

        free_minefield(current_field);
    }

    return cleared_area;
}

/**
 * Recursively generates all binary permutations of a given array, representing the possible mines in a minefield.
 * Each generated permutation is stored in the given permutation array.
 * @param remaining_mine_amount amount of mines in a permutation.
 * @param mine_sum amount of mines in total before clearing (length of a permutation).
 * @param permutation dynamic permutation, changes as each permutation is generated.
 * @param permutations array to store permutations
 * @return amount of generated permutations.
 */
int generate_permutations(int remaining_mine_amount, int mine_sum, int* permutation, int** permutations) {
    int current_mine = -1;
    int mine_count = 0;
    int result = 0;

    // Set mine count and current mine
    for (int mine = 0; mine < mine_sum; ++mine) {
        if (permutation[mine] != -1) {
            if (permutation[mine] == 1) {
                mine_count++;
            }
        }
        if (permutation[mine] == -1) {
            current_mine = mine;
            break;
        }
    }

    if (current_mine == -1 && mine_count == remaining_mine_amount) {
        // Make copy of current valid permutation
        int* permutation_copy = malloc(sizeof(int)*mine_sum);
        for (int mine = 0; mine < mine_sum; ++mine) {
            permutation_copy[mine] = permutation[mine];
        }

        // Copy insert copy in first available space in the permutations array
        int permutation_amount = 0;
        while (permutations[permutation_amount] != NULL) {
            permutation_amount++;
        }
        permutations[permutation_amount] = permutation_copy;
        return 1;
    } else if (current_mine != -1 && mine_count <= remaining_mine_amount) {
        // Generate next two permutations, one where the current mine is set to 1, the other set to 0
        for (int is_mine = 0; is_mine < 2; ++is_mine) {
            permutation[current_mine] = is_mine;
            result += generate_permutations(remaining_mine_amount, mine_sum, permutation, permutations);
            permutation[current_mine] = -1;
        }
        return result;
    } else {
        return 0;
    }
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
