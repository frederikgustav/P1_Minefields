#include "minefields.h"

struct sub_minefield get_biggest_cleared_sub_minefield(struct minefield field);
struct sub_minefield get_biggest_clearable_sub_minefield(struct minefield field, int mine_capacity);
struct sub_minefield get_biggest_sub_minefield_from_permutation(struct minefield field, const int* permutation);

int generate_permutations(struct minefield field, int remaining_mine_amount, int mine_sum, int* permutation, int* best_permutation);

int get_sub_minefield_area(struct sub_minefield area);

