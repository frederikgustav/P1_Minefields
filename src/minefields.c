#include <stdlib.h>
#include <stdio.h>
#include "minelib.h"

int main(void) {
    struct minefield field = get_random_minefield(5, 5, 1, 8);

    return EXIT_SUCCESS;
}
