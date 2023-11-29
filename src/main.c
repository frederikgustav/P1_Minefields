#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "minefield_algorithms.h"

int main(void) {
    // Seed random generator
    srand(time(NULL));

    struct minefield field = get_empty_minefield(4, 4, 1.0);
    field.matrix[0][0].mine = 1;
    field.matrix[1][2].mine = 1;
    field.matrix[1][3].mine = 1;
    field.matrix[2][1].mine = 1;
    field.matrix[3][3].mine = 1;
    struct sub_minefield area = get_biggest_clearable_sub_minefield(field, 2);
    //struct sub_minefield area = get_biggest_cleared_sub_minefield(field);
    print_minefield(field);

    printf("Biggest clearable sub minefield:\n");
    printf("Start point: (%d, %d)\n", area.start_point.x, area.start_point.y);
    printf("End point: (%d, %d)\n", area.end_point.x, area.end_point.y);

    free_minefield(field);
    return EXIT_SUCCESS;
}
