struct square {
    int mine; //bool
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

void print_minefield(struct minefield field);
struct minefield get_empty_minefield(int width, int height, double metric_square_length);
struct cleared_area clear_area(struct minefield field, int mine_removal_capacity);
struct minefield get_random_minefield(int width, int height, double metric_square_length, int mine_amount);
void free_minefield(struct minefield* field);
