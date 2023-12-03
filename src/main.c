#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "minefield_algorithms.h"

int main(void) {
    // Seed random generator
    srand(time(NULL));

    minefield field = get_random_minefield(8, 8, 14);
    print_minefield(field);
    printf("\n");

    zone zone = get_biggest_clearable_zone(field, 4);

    print_minefield(field);
    print_minefield_zone(field, zone);

    return EXIT_SUCCESS;
}
