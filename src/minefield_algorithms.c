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
 * Gives biggest clearable zone in a minefield (wraps check_minefield_permutations)
 * @param field the minefield to check for largest clearable zone
 * @param mine_removal_capacity the amount of mines that can be cleared
 * @return biggest clearable zone
 */
zone get_biggest_clearable_zone(minefield field, int mine_removal_capacity) {
    zone biggest_clearable_zone = get_biggest_cleared_zone(field);
    int final_mine_count = get_minefield_sum(field) - mine_removal_capacity;
    check_minefield_permutations(field, &biggest_clearable_zone, 0, final_mine_count);

    return biggest_clearable_zone;
}

/**
 * Recursively finds the biggest clearable zone in a minefield
 * @param field the minefield
 * @param best_zone output parameter for the best clearable zone
 * @param index the current index in the minefield
 * @param final_mine_count the amount of mines in each valid permutation
 * @return the amount of valid permutations
 */
int check_minefield_permutations(minefield field, zone* best_zone, int index, int final_mine_count) {
    int result = 0;
    int x = index % field.width;
    int y = index / field.width;

    if (index == field.height * field.width) {
        if (get_minefield_sum(field) == final_mine_count) {
            zone biggest_cleared_zone = get_biggest_cleared_zone(field);
            int best_clearable_zone_area = get_zone_area(*best_zone);
            int current_zone_area = get_zone_area(biggest_cleared_zone);

            if (current_zone_area > best_clearable_zone_area) {
                *best_zone = biggest_cleared_zone;
                return 1;
            }  else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (field.matrix[y][x].mine != 1) {
        return check_minefield_permutations(field, best_zone, index + 1, final_mine_count);
    } else {
        result += check_minefield_permutations(field, best_zone, index + 1, final_mine_count);
        field.matrix[y][x].mine = 0;
        result += check_minefield_permutations(field, best_zone, index, final_mine_count);
        field.matrix[y][x].mine = 1;

        return result;
    }
}

/**
 * Combines binary and expansion zoning to approximate the best clearable zone
 * @param field the minefield
 * @param mine_capacity the amount of mines that can be cleared
 * @return the best approximate zone
 */
zone quick_clear(minefield field, int mine_capacity) {
    zone zone = binary_zoning(field, mine_capacity);
    zone = expansion_zoning(field, mine_capacity, zone);
    return zone;
}

/**
 * Divides the minefield into 4 zones until the mine capacity is reached
 * @param field the minefield
 * @param mine_capacity the amount of mines that can be cleared
 * @param current_zone the zone to start with
 * @return the best approximate zone
 */
zone binary_zoning(minefield field, int mine_capacity) {
    zone current_zone = {{0,0}, {field.width - 1, field.height - 1}};
    while (mine_capacity < get_zone_mine_sum(field, current_zone)) {
        int midX = (current_zone.start.x + current_zone.end.x) / 2;
        int midY = (current_zone.start.y + current_zone.end.y) / 2;

        zone up_r = {current_zone.start, {midX, midY}};
        zone low_r = {{current_zone.start.x, midY + 1},{midX,current_zone.end.y}};
        zone up_l = {{midX + 1, current_zone.start.y},{current_zone.end.x, midY}};
        zone low_l = {{midX + 1, midY + 1}, current_zone.end};

        // find the zone with the least mines
        int up_r_count = get_zone_mine_sum(field, up_r);
        int low_r_count = get_zone_mine_sum(field, low_r);
        int up_l_count = get_zone_mine_sum(field, up_l);
        int low_l_count = get_zone_mine_sum(field, low_l);

        // select lowest count zone
        if (up_r_count <= low_r_count && up_r_count <= up_l_count && up_r_count <= low_l_count) {
            current_zone = up_r;
        } else if (low_r_count <= up_r_count && low_r_count <= up_l_count && low_r_count <= low_l_count) {
            current_zone = low_r;
        } else if (up_l_count <= up_r_count && up_l_count <= low_r_count && up_l_count <= low_l_count) {
            current_zone = up_l;
        } else {
            current_zone = low_l;
        }
    }
    return current_zone;
}

/**
 * Expands the given zone until the mine capacity is reached, or until the zone cannot be expanded more
 * @param field the minefield
 * @param mine_capacity the amount of mines that can be cleared
 * @param current_zone the zone to start with
 * @return the best approximate zone
 */
zone expansion_zoning(minefield field, int mine_capacity, zone current_zone) {
    // generate zones expanded to the left, right, up and down
    zone left_zone = {{current_zone.start.x - 1, current_zone.start.y}, {current_zone.end.x, current_zone.end.y}};
    zone right_zone = {{current_zone.start.x, current_zone.start.y}, {current_zone.end.x + 1, current_zone.end.y}};
    zone up_zone = {{current_zone.start.x, current_zone.start.y - 1}, {current_zone.end.x, current_zone.end.y}};
    zone down_zone = {{current_zone.start.x, current_zone.start.y}, {current_zone.end.x, current_zone.end.y + 1}};

    // get counts
    int left_count = get_zone_mine_sum(field, left_zone);
    int right_count = get_zone_mine_sum(field, right_zone);
    int up_count = get_zone_mine_sum(field, up_zone);
    int down_count = get_zone_mine_sum(field, down_zone);

    // get zone areas
    int left_area = get_zone_area(left_zone);
    int right_area = get_zone_area(right_zone);
    int up_area = get_zone_area(up_zone);
    int down_area = get_zone_area(down_zone);

    // calculate factors for each zone
    double left_factor = (double) left_count / left_area;
    double right_factor = (double) right_count / right_area;
    double up_factor = (double) up_count / up_area;
    double down_factor = (double) down_count / down_area;

}
