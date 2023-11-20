#include "minelib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct minefield get_empty_minefield(int width, int height, double metric_square_length) {
    struct minefield field;

    field.height = height;
    field.width = width;
    field.metric_square_length = metric_square_length;
    field.matrix = malloc(sizeof(struct square**)*height);

    for (int i = 0; i < height; ++i) {
        field.matrix[i] = malloc(sizeof(struct square*)*width);
        for (int j = 0; j < width; ++j) {
            field.matrix[i][j].mine = 0;
        }
    }

    return field;
}

struct minefield get_random_minefield(int width, int height, double metric_square_length, int mine_amount) {
    struct minefield field = get_empty_minefield(width, height, metric_square_length);
    srand(time(NULL));

    // For-loop generating random mines
    for (int i = 0; i < mine_amount; ++i) {
        int random_y = rand() % height;
        int random_x = rand() % width;

        if (field.matrix[random_y][random_x].mine) {
            i--;
            continue;
        }

        field.matrix[random_y][random_x].mine = 1;
    }

    return field;
}

void print_minefield(struct minefield field) {
  for (int i = 0; i < field.height; ++i) {
    printf("| ");

    for (int j = 0; j < field.width; ++j) {
      if (field.matrix[i][j].mine == 0) {
        printf("O ");
      } else {
        printf("X ");
      }
    }

    printf("| \n");
  }
}
