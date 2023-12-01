#include "minefields.h"

zone get_biggest_cleared_zone(minefield field);
zone get_biggest_clearable_zone(minefield field, int mine_capacity);
zone get_biggest_cleared_zone_from_permutation(minefield field, const int* permutation);

int check_permutations(minefield field, int final_mine_count, int start_mine_count, int* permutation, int* best_permutation);
