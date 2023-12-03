#include "minefields.h"

zone get_biggest_cleared_zone(minefield field);
zone get_biggest_clearable_zone(minefield field, int mine_removal_capacity);
int check_minefield_permutations(minefield field, zone* best_clearable_zone, int current_index, int final_mine_count);

zone quick_clear(minefield field, int mine_capacity);
zone binary_zoning(minefield field, int mine_capacity);
zone expansion_zoning(minefield field, int mine_capacity, zone current_zone);