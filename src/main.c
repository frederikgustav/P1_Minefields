#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "minefield_algorithms.h"

int main(void) {
    // Seed random generator
    srand(time(NULL));

    int mines_removal_capacity = 1;
    int mines = 50;
    minefield field = get_random_minefield(10, 10, mines);
    zone start_zone = {{0, 0}, {9, 9}};

    printf("Minefield: \n");
    print_minefield(field);

    zone zone1 = binary_zoning(field, mines_removal_capacity, start_zone);

    printf("Zone: \n");
    print_minefield_zone(field, zone1);

    free_minefield(field);
    return EXIT_SUCCESS;
}
