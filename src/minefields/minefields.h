/*
 * Represents a point in a known minefield.
 */
typedef struct point {
    int x;
    int y;
} point;

/*
 * Represents a single square in minefield.
 */
typedef struct square {
    int mine; // bool
} square;

/*
 * Represents a minefield, with a matrix of squares.
 */
typedef struct minefield {
    int width;
    int height;
    square** matrix;
} minefield;

/*
 * Represents a zone in a minefield, with a start and end point.
 */
typedef struct zone {
    point start;
    point end;
} zone;

minefield get_empty_minefield(int width, int height);
minefield get_random_minefield(int width, int height, int mine_count);

void print_minefield_zone(minefield field, zone zone);
void print_minefield(minefield field);
void free_minefield(minefield field);

int get_minefield_sum(struct minefield field);
int get_zone_area(zone zone);
int get_zone_mine_sum(minefield field, zone zone);
int get_zone_height(zone zone);
int get_zone_width(zone zone);
double get_zone_mine_density(minefield field, zone zone);

int is_valid_zone(minefield field, zone zone);