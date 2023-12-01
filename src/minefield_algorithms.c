#include <stdio.h>
#include <stdlib.h>
#include "minefield_algorithms.h"

/**
 * Gives biggest cleared zone in a minefield
 * @param field the minefield to check for largest cleared zone
 * @return biggest cleared zone
 */
zone get_biggest_cleared_zone(minefield field) {
    int max_zone = 0;
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
                int zone = minHeight * (x - k + 1);
                if (zone > max_zone) {
                    max_zone = zone;
                    max_left = k;
                    max_right = x;
                    max_top = y - minHeight + 1;
                    max_bottom = y;
                }
            }
        }
    }

    zone biggest_rect;
    biggest_rect.start.x = max_left;
    biggest_rect.start.y = max_top;
    biggest_rect.end.x = max_right;
    biggest_rect.end.y = max_bottom;

    return biggest_rect;
}

/**
 * Gives biggest clearable zone in a minefield
 * @param field the minefield to check for largest clearable zone
 * @param mine_capacity the amount of mines that can be cleared
 * @return biggest clearable zone
 */
zone get_biggest_clearable_zone(minefield field, int mine_capacity) {
    int start_mine_count = get_minefield_sum(field);
    int final_mine_count = start_mine_count - mine_capacity;

    // Create seed permutation, example: [-1, -1, -1, -1]
    int* permutation = malloc(sizeof(int) * start_mine_count);
    for (int mine = 0; mine < start_mine_count; ++mine) {
        permutation[mine] = -1;
    }

    // Create int array to store best permutation, initialized as all 1s, example: [1, 1, 1, 1]
    int* best_permutation = malloc(sizeof(int) * start_mine_count);
    for (int mine = 0; mine < start_mine_count; ++mine) {
        best_permutation[mine] = 1;
    }

    int permutation_amount = check_permutations(field, final_mine_count, start_mine_count, permutation, best_permutation);
    printf("Permutation amount: %d\n", permutation_amount);

    return get_biggest_cleared_zone_from_permutation(field, best_permutation);
}

/**
 * Gives biggest cleared zone in a minefield from a permutation, example: [1, 0, 1, 0, 0, 1]
 * @param field the original minefield, on which the permutation can be applied
 * @param permutation the permutation to check
 * @return biggest cleared zone
 */
zone get_biggest_cleared_zone_from_permutation(minefield field, const int* permutation) {
    int current_mine = 0;

    // Map permutation to minefield
    struct minefield current_field = get_empty_minefield(field.width, field.height);
    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            if (field.matrix[y][x].mine == 1) {
                current_field.matrix[y][x].mine = permutation[current_mine];
                current_mine++;
            }
        }
    }

    // Get cleared area
    zone cleared_zone = get_biggest_cleared_zone(current_field);
    free_minefield(current_field);

    return cleared_zone;
}

/**
 * Checks all permutations of a minefield
 * @param field the minefield to check
 * @param final_mine_count the amount of mines that should be left after clearing
 * @param start_mine_count the amount of mines in the minefield, also the length the permutations
 * @param permutation the current permutation
 * @param best_permutation the best permutation
 * @return amount of permutations checked
 */
int check_permutations(minefield field, int final_mine_count, int start_mine_count, int* permutation, int* best_permutation) {
    int current_mine = -1;
    int mine_count = 0;
    int result = 0;

    // Set mine count and current mine
    for (int mine = 0; mine < start_mine_count; ++mine) {
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

    if (current_mine == -1 && mine_count == final_mine_count) {
        // Check if current permutation is better than the best permutation
        zone current_zone = get_biggest_cleared_zone_from_permutation(field, permutation);
        zone best_zone = get_biggest_cleared_zone_from_permutation(field, best_permutation);
        int current_area = get_zone_area(current_zone);
        int best_area = get_zone_area(best_zone);

        if (current_area > best_area) {
            for (int mine = 0; mine < start_mine_count; ++mine) {
                best_permutation[mine] = permutation[mine];
            }
        }
        return 1;
    } else if (current_mine != -1 && mine_count <= final_mine_count) {
        // Generate next two permutations, one where the current mine is set to 1, the other set to 0
        for (int is_mine = 0; is_mine < 2; ++is_mine) {
            permutation[current_mine] = is_mine;
            result += check_permutations(field, final_mine_count, start_mine_count, permutation, best_permutation);
            permutation[current_mine] = -1; // maybe not needed
        }
        return result;
    } else {
        return 0;
    }
}
