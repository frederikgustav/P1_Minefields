#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "minefield_algorithms.h"

int main(void) {
    // Seed random generator
    srand(time(NULL));
    double time_spent = 0.0;

    int mines_removal_capacity = 7;
    int mines = 20;
    minefield field = get_random_minefield(10, 10, mines);

    clock_t c0 = clock();
    zone zone = get_biggest_clearable_zone(field, mines_removal_capacity);
    clock_t c1 = clock();
    double runtime_diff_ms = (c1 - c0) * 1000. / CLOCKS_PER_SEC;
    // convert runtime_diff_ms to seconds
    runtime_diff_ms /= 1000;

    printf("Minefield:\n");
    print_minefield(field);
    printf("Biggest clearable sub minefield, if %d mines can be cleared:\n", mines_removal_capacity);
    print_minefield_zone(field, zone);


    printf("Runtime: %f s\n", runtime_diff_ms);

    free_minefield(field);

    return EXIT_SUCCESS;
}
