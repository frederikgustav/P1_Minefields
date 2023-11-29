#include <stdlib.h>
#include <assert.h>

#include "minefields.h"

void test_get_empty_minefield();
void test_get_random_minefield();

int main(void) {
    test_get_empty_minefield();
    test_get_random_minefield();

    return EXIT_SUCCESS;
}

void test_get_empty_minefield() {
    /* Arrange */
    int width = 5;
    int height = 5;
    int metric_square_length = 1;

    /* Act */
    struct minefield field = get_empty_minefield(width, height, metric_square_length);

    /* Assert */
    // Nested for-loop to check matrix positions for mines
    // "== 0" to check that no mine is in [x][y] position
    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            assert(field.matrix[y][x].mine == 0);
        }
    }
}

void test_get_random_minefield() {
    /* Arrange */
    //Variables for Act
    int width = 5;
    int height = 5;
    int metric_square_length = 1;
    int amount = 5;

    //Variable for Assert
    int assert_counter = 0;

    /* Act */
    struct minefield field = get_random_minefield(width, height, metric_square_length, amount);

    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            if (field.matrix[y][x].mine == 1){
                ++assert_counter;
            }
        }
    }

    /* Assert */
    assert(assert_counter == amount);
}
