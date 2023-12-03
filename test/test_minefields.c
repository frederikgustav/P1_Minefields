#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "minefield_algorithms.h"

void test_get_empty_minefield();
void test_get_random_minefield();
void test_get_zone_mine_sum();

int main(void) {
    // Seed random generator
    srand(time(NULL));

    test_get_empty_minefield();
    test_get_random_minefield();
    test_get_zone_mine_sum();

    return EXIT_SUCCESS;
}

void test_get_empty_minefield() {
    /* Arrange */
    int width = 5;
    int height = 5;

    /* Act */
    minefield field = get_empty_minefield(width, height);

    /* Assert */
    // Nested for-loop to check matrix positions for mines
    // "== 0" to check that no mine is in [y][x] position
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
    int amount = 5;

    //Variable for Assert
    int assert_counter = 0;

    /* Act */
    minefield field = get_random_minefield(width, height, amount);

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

void test_get_zone_mine_sum() {
    /* Arrange */
    zone zone = {{0,0}, {1,1}};
    minefield field = get_empty_minefield(5, 5);

    // Inside zone
    field.matrix[0][0].mine = 1;
    field.matrix[0][1].mine = 1;
    field.matrix[1][0].mine = 1;
    field.matrix[1][1].mine = 1;

    // Outside zone
    field.matrix[2][2].mine = 1;
    field.matrix[3][3].mine = 1;

    /* Act */
    int area = get_zone_mine_sum(field, zone);

    /* Assert */
    assert(area == 4);
}
