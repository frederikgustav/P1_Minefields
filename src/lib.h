struct square {
    int mine; // bool
    double cost;
    double elevation;
};

struct minefield {
    int width;
    int height;
    struct square** matrix;
    double metric_square_length;
};
