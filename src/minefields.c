#include <stdlib.h>
#include <stdio.h>
#include "minelib.h"

int main(void) {
    struct minefield field = get_random_minefield(100, 100, 1, 4);

    int amount = clear_area(field, 2);
    printf("%d", amount);

    free_minefield(field);
    return EXIT_SUCCESS;
}
