#include <stdlib.h>
#include <assert.h>

#include "..\src\minelib.h"

void test_get_empty_minefield();
void test_get_random_minefield();

int main(void) {
    test_get_empty_minefield();

    return EXIT_SUCCESS;
}

void test_get_empty_minefield() {
    struct minefield field = get_empty_minefield(5, 5, 1);

    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            assert(field.matrix[y][x].mine == 0);
        }
    }
}

void test_get_random_minefield() {
    struct minefield field = get_random_minefield(5, 5, 1, 5);

}