#include "../minefields/minefields.h"

zone get_biggest_cleared_zone(minefield field);
zone get_biggest_clearable_zone(minefield field, int mine_removal_capacity);
int check_minefield_permutations(minefield field, zone* best_zone, int index, int final_mine_count);

int minefield_permutation_possibly_valid(minefield field, int final_mine_count, int max_x, int max_y);

zone center_expansion(minefield field, int mine_capacity);

zone quick_clear(minefield field, int mine_capacity);
zone binary_zoning(minefield field, int mine_capacity);
zone expansion_zoning(minefield field, int mine_capacity, zone current_zone);