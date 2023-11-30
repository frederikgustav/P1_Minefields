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

void print_sub_minefield_in_minefield(struct minefield field, struct sub_minefield sub_field);
void print_minefield(struct minefield field);
void free_minefield(struct minefield field);

int get_minefield_sum(struct minefield field);
