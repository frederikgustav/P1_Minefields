#include <stdlib.h>
#include <stdio.h>
#include "minefield_algorithms.h"

int min(int a, int b);
int factorial(int n);

/**
 * Gives biggest cleared square in minefield
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
 * Gives biggest clearable area in a minefield
 * @param field
 * @param mine_capacity
 * @return
 */
struct sub_minefield get_biggest_clearable_sub_minefield(struct minefield field, int mine_capacity) {
    int mine_sum = get_minefield_sum(field);
    int permutation_amount = factorial(mine_sum)/(factorial(mine_capacity)*factorial(mine_sum-mine_capacity));

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
    struct sub_minefield max_cleared_area = {
        .start_point = {0, 0},
        .end_point = {0, 0},
    };
    struct sub_minefield current_cleared_area;

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

        // Get cleared area
        current_cleared_area = get_biggest_cleared_sub_minefield(current_field);

        // Update max cleared area
        if (get_sub_minefield_area(current_cleared_area) > get_sub_minefield_area(max_cleared_area)) {
            max_cleared_area = current_cleared_area;
        }

        free_minefield(current_field);
    }

    return max_cleared_area;
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
 * Gives the minimum of a and b. If a is less than b, it returns a; otherwise, it returns b.
 * @param a
 * @param b
 * @return the smallest number
 */
int min(int a, int b) {
    return (a < b) ? a : b;
}

/**
 * Gives factorial of a number
 * @param n number
 * @return result
 */
int factorial(int n) {
    int result = n;
    for (int i = n-1; i >= 1; --i) {
        result *= i;
    }
    return result;
}

/**
 * Gives the area of a sub minefield
 * @param area the sub minefield
 * @return
 */
int get_sub_minefield_area(struct sub_minefield area) {
    return (area.end_point.x - area.start_point.x + 1) * (area.end_point.y - area.start_point.y + 1);
}
