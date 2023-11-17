#include "minelib.h"
#include <malloc.h>
#include <stdio.h>

#define MINE_COST 300

struct minefield get_empty_minefield(int width, int height, double metric_square_length) {
    struct minefield field;

    field.height = height;
    field.width = width;
    field.metric_square_length = metric_square_length;
    field.matrix = malloc(sizeof(struct square**)*height);

    for (int i = 0; i < height; ++i) {
        field.matrix[i] = malloc(sizeof(struct square*)*width);
        for (int j = 0; j < width; ++j) {
            field.matrix[i][j].cost = 0;
        }
    }

    return field;
}

struct minefield get_random_minefield(int width, int height, double metric_square_length, int mine_amount) {
    // return randomized minefield
    printf("");
}

void print_minefield(struct square** field) {
    // elma print et minefelt
    return; // remove
}
