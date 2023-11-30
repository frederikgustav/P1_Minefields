#include <stdio.h>
#include <stdlib.h>
#include "minefield_algorithms.h"

/**
 * Gives biggest cleared square in minefield
 * @param field the parameter field is used to provide information about the generated minefield for this function
 * to find the largest possible area within the minefield that can be cleared (that has as many mines as possible)
 * @return it retures the area that contains the most mines
 */
struct sub_minefield get_biggest_cleared_sub_minefield(struct minefield field) {
    int max_area = 0;
    int max_left = 0, max_right = 0, max_top = 0, max_bottom = 0;
    int height[field.height][field.width];

    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            if (field.matrix[y][x].mine == 1) {
                height[y][x] = 0;
            } else {
                height[y][x] = (y == 0) ? 1 : height[y - 1][x] + 1;
            }
        }
    }

    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            int minHeight = height[y][x];
            for (int k = x; k >= 0; --k) {
                minHeight = minHeight < height[y][k] ? minHeight : height[y][k];
                int area = minHeight * (x - k + 1);
                if (area > max_area) {
                    max_area = area;
                    max_left = k;
                    max_right = x;
                    max_top = y - minHeight + 1;
                    max_bottom = y;
                }
            }
        }
    }

    struct sub_minefield biggest_rect;
    biggest_rect.start_point.x = max_left;
    biggest_rect.start_point.y = max_top;
    biggest_rect.end_point.x = max_right;
    biggest_rect.end_point.y = max_bottom;

    return biggest_rect;
}

/**
 * Gives biggest clearable area in a minefield
 * @param field
 * @param mine_capacity
 * @return
 */
struct sub_minefield get_biggest_clearable_sub_minefield(struct minefield field, int mine_capacity) {
    int mine_sum = get_minefield_sum(field);

    // Create seed permutation
    int* permutation = malloc(sizeof(int) * mine_sum);
    for (int mine = 0; mine < mine_sum; ++mine) {
        permutation[mine] = -1;
    }

    // Create best permutation
    int* best_permutation = malloc(sizeof(int) * mine_sum);
    for (int mine = 0; mine < mine_sum; ++mine) {
        best_permutation[mine] = 1; // 1 to prioritize this the least
    }
    int permutation_amount = generate_permutations(field, mine_sum-mine_capacity, mine_sum, permutation, best_permutation);
    printf("Permutation amount: %d\n", permutation_amount);

    return get_biggest_sub_minefield_from_permutation(field, best_permutation);
}

struct sub_minefield get_biggest_sub_minefield_from_permutation(struct minefield field, const int* permutation) {
    int current_mine = 0;

    // Map permutation to minefield
    struct minefield current_field = get_empty_minefield(field.width, field.height, field.metric_square_length);
    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            if (field.matrix[y][x].mine == 1) {
                current_field.matrix[y][x].mine = permutation[current_mine];
                current_mine++;
            }
        }
    }

    // Get cleared area
    struct sub_minefield cleared_area = get_biggest_cleared_sub_minefield(current_field);
    free_minefield(current_field);

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
int generate_permutations(struct minefield field, int remaining_mine_amount, int mine_sum, int* permutation, int* best_permutation) {
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
        // Check if current permutation is better than the best permutation
        struct sub_minefield current_sub_minefield = get_biggest_sub_minefield_from_permutation(field, permutation);
        struct sub_minefield best_sub_minefield = get_biggest_sub_minefield_from_permutation(field, best_permutation);
        int current_area = get_sub_minefield_area(current_sub_minefield);
        int best_area = get_sub_minefield_area(best_sub_minefield);

        if (current_area > best_area) {
            for (int mine = 0; mine < mine_sum; ++mine) {
                best_permutation[mine] = permutation[mine];
            }
        }
        return 1;
    } else if (current_mine != -1 && mine_count <= remaining_mine_amount) {
        // Generate next two permutations, one where the current mine is set to 1, the other set to 0
        for (int is_mine = 0; is_mine < 2; ++is_mine) {
            permutation[current_mine] = is_mine;
            result += generate_permutations(field, remaining_mine_amount, mine_sum, permutation, best_permutation);
            permutation[current_mine] = -1;
        }
        return result;
    } else {
        return 0;
    }
}

/**
 * Gives the area of a sub minefield
 * @param area the sub minefield
 * @return
 */
int get_sub_minefield_area(struct sub_minefield area) {
    return (area.end_point.x - area.start_point.x + 1) * (area.end_point.y - area.start_point.y + 1);
}

/**
 * Gives the amount of mines in a specific area in a minefield
 * @param field
 * @param area
 * @return mine amount
 */
int get_sub_minefield_mine_amount(struct minefield field, struct sub_minefield area) {
    int mine_amount = 0;
    for (int y = area.start_point.y; y <= area.end_point.y; ++y) {
        for (int x = area.start_point.x; x <= area.end_point.x; ++x) {
            if (field.matrix[y][x].mine == 1) {
                mine_amount++;
            }
        }
    }
    return mine_amount;
}
