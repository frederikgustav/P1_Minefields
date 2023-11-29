/*
 * Represents a point in a known minefield.
 */
struct point {
    int x;
    int y;
};

/*
 * Represents a single square in minefield.
 */
struct square {
    int mine; //bool
};

/*
 * Represents a minefield, with a matrix of squares.
 */
struct minefield {
    int width;
    int height;
    struct square** matrix; // 2d array
    double metric_square_length; // height and length in meters
};

/*
 * Represents a sub minefield
 */
struct sub_minefield {
    struct point start_point;
    struct point end_point;
};

struct minefield get_empty_minefield(int width, int height, double metric_square_length);
struct minefield get_random_minefield(int width, int height, double metric_square_length, int mine_amount);
struct minefield get_minefield_copy(struct minefield field);
struct minefield get_sub_minefield_copy(struct minefield field, struct point start_point, struct point end_point); // needs implementing
struct sub_minefield get_biggest_cleared_sub_minefield(struct minefield field);

int get_minefield_sum(struct minefield field);

struct sub_minefield get_biggest_clearable_sub_minefield(struct minefield field, int mine_capacity);
int generate_permutations(int remaining_mine_amount, int mine_sum, int* permutation, int** permutations);

void print_minefield(struct minefield field);
void free_minefield(struct minefield field);
