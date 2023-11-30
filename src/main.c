#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "minefield_algorithms.h"

int main(void) {
    // Seed random generator
    srand(time(NULL));

    int mines_removal_capacity = 5;
    struct minefield field = get_random_minefield(20, 20, 1, 12);
    struct sub_minefield area = get_biggest_clearable_sub_minefield(field, mines_removal_capacity);

    printf("Minefield:\n");
    print_minefield(field);
    printf("Biggest clearable sub minefield, if %d mines can be cleared:\n", mines_removal_capacity);
    print_sub_minefield_in_minefield(field, area);

    free_minefield(field);
    return EXIT_SUCCESS;
}
