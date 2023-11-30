#include "minefields.h"

struct sub_minefield get_biggest_cleared_sub_minefield(struct minefield field);
struct sub_minefield get_biggest_clearable_sub_minefield(struct minefield field, int mine_capacity);
int get_sub_minefield_area(struct sub_minefield area);

int generate_permutations(int remaining_mine_amount, int mine_sum, int* permutation, int** permutations);
