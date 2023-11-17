struct square {
    double cost;
};

struct minefield {
    int width;
    int height;
    struct square** matrix;
    double metric_square_length;
};

struct cleared_area {
    int remaining_resources;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
};

void print_minefield(struct square** field);
struct minefield get_empty_minefield(int width, int height, double metric_square_length);
struct cleared_area clear_area(struct minefield field, double resources);
struct minefield get_random_minefield(int width, int height, double metric_square_length, int mine_amount);
