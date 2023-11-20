#include <stdlib.h>
#include <stdio.h>
#include "minelib.h"

int main(void) {
    struct minefield field = get_random_minefield(20, 20, 1, 100);
    print_minefield(field);
    free_minefield(&field);

    return EXIT_SUCCESS;
}
