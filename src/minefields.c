#include <stdlib.h>
#include <stdio.h>
#include "minelib.h"

int main(void) {
    struct minefield field = get_empty_minefield(5, 5, 1);

    printf("%lf", field.matrix[3][3].cost);

    return EXIT_SUCCESS;
}
