#include <stdlib.h>
#include <stdio.h>
#include "minelib.h"

int main(void) {
    struct minefield field = get_random_minefield(20, 20, 1, 100);
    print_minefield(field);
    struct sub_minefield biggest_square = get_biggest_cleared_sub_minefield(field);

    printf("Coordinates of the biggest free square: \n");
    printf("Start Point: (%d, %d)\n", biggest_square.start_point.x, biggest_square.start_point.y);
    printf("End Point: (%d, %d)\n", biggest_square.end_point.x, biggest_square.end_point.y);

    free_minefield(field);
    return EXIT_SUCCESS;
}
