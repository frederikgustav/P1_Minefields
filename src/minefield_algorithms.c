#include <stdio.h>
#include <stdlib.h>
#include "minefield_algorithms.h"

/**
 * Gives biggest cleared square in minefield
 * @param field the parameter field is used to provide information about the generated minefield for this function
 * to find the largest possible area within the minefield that can be cleared (that has as many mines as possible)
 * @return it retures the area that contains the most mines
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
 * Gives biggest clearable area in a minefield
 * @param field
 * @param mine_capacity
 * @return
 */
zone get_biggest_clearable_zone(minefield field, int mine_capacity) {
    int start_mine_count = get_minefield_sum(field);
    int final_mine_count = start_mine_count - mine_capacity;

    // Create seed permutation
    int* permutation = malloc(sizeof(int) * start_mine_count);
    for (int mine = 0; mine < start_mine_count; ++mine) {
        permutation[mine] = -1;
    }

    // Create best permutation
    int* best_permutation = malloc(sizeof(int) * start_mine_count);
    for (int mine = 0; mine < start_mine_count; ++mine) {
        best_permutation[mine] = 1; // 1 to prioritize this the least
    }

    int permutation_amount = check_permutations(field, final_mine_count, start_mine_count, permutation, best_permutation);
    printf("Permutation amount: %d\n", permutation_amount);

    return get_biggest_cleared_zone_from_permutation(field, best_permutation);
}

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

zone binary_zoning(minefield field, int mine_capacity, zone start_zone){
    // Base case: If there are no mines left to clear or only one cell in the zone
    if (mine_capacity <= 0 || start_zone.start.x > start_zone.end.x || start_zone.start.y > start_zone.end.y) {
        return start_zone;
    }

    // Check mines in the current quadrant
    for (int y = start_zone.start.y; y <= start_zone.end.y; ++y) {
        for (int x = start_zone.start.x; x < start_zone.end.x; ++x) {
            if (field.matrix[y][x].mine){
                field.matrix[y][x].mine = 0;
                mine_capacity--;
            }
        }
    }
    // Divide the current zone into two part horizontally or vertically
    int midX = (start_zone.start.x + start_zone.end.x) / 2;
    int midY = (start_zone.start.y + start_zone.end.y) / 2;

    zone upper_zone = {start_zone.start, {midX, midY}};
    zone lower_zone = {{start_zone.start.x, midY + 1}, start_zone.end};
    zone left_zone = {start_zone.start, {midX, start_zone.end.y}};
    zone right_zone = {{midX + 1, start_zone.start.y}, start_zone.end};

    // Recursively performing binary zoning in each sub-zone
    zone upper_best = binary_zoning(field, mine_capacity, upper_zone);
    zone lower_best = binary_zoning(field, mine_capacity, lower_zone);
    zone left_best = binary_zoning(field, mine_capacity, left_zone);
    zone right_best = binary_zoning(field, mine_capacity, right_zone);

    // Compare the cleared areas in each sub-zone and return the best one
    int upper_area = get_zone_area(upper_best);
    int lower_area = get_zone_area(lower_best);
    int left_area = get_zone_area(left_best);
    int right_area = get_zone_area(right_best);

    zone best_zone = start_zone; // Default back to current zone.
    int best_area = get_zone_area(best_zone);

    if (upper_area > best_area) {
        best_zone = upper_best;
        best_area = upper_area;
    }
    if (lower_area > best_area) {
        best_zone = lower_best;
        best_area = lower_area;
    }
    if (left_area > best_area) {
        best_zone = left_best;
        best_area = left_area;
    }
    if (right_area > best_area) {
        best_zone = right_best;
        best_area = right_area;
    }

    return best_zone;

}
