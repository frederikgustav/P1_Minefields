#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "../src/minefield_algorithms/minefield_algorithms.h"

void test_get_empty_minefield();
void test_get_random_minefield();
void test_get_zone_mine_sum();
void test_get_zone_area();
void test_get_minefield_sum();
void test_get_biggest_cleared_zone();
void test_get_biggest_clearable_zone();

int main(void) {
    // Seed random generator
    srand(time(NULL));

    test_get_empty_minefield();
    test_get_random_minefield();
    test_get_zone_mine_sum();
    test_get_zone_area();
    test_get_minefield_sum();
    test_get_biggest_cleared_zone();
    test_get_biggest_clearable_zone();

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

    // Variable for Assert
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
    //Variables for Act
    int width = 5;
    int height = 5;
    minefield field = get_empty_minefield(width, height);

    // Place mines at specific positions
    field.matrix[0][0].mine = 1;
    field.matrix[1][2].mine = 1;
    field.matrix[2][3].mine = 1;
    field.matrix[3][1].mine = 1;
    field.matrix[3][3].mine = 1;
    field.matrix[4][4].mine = 1;

    zone test_zone = {{2, 2}, {4, 4}};

    /* Act */
    int result = get_zone_mine_sum(field, test_zone);

    /* Assert */
    assert(result == 3);  // Adjust the expected value based on your minefield matrix and zone
}

void test_get_zone_area() {
    /* Arrange */
    //Variables for Act
    zone test_zone = {{1, 1}, {4, 4}};

    /* Act */
    int result = get_zone_area(test_zone);

    /* Assert */
    //Calculate the area manually to check if function calculates it correctly.
    assert(result == 16);
}

// test_get_biggest_cleared_zone()
// test_get_biggest_clearable_zone()
// test_binary_zoning()
void test_get_minefield_sum() {
    /* Arrange */
    //Variables for Act
    int width = 5;
    int height = 5;
    int amount = 5;

    // Variable for Assert
    int mine_counter;

    /* Act */
    minefield field = get_random_minefield(width, height, amount);

    mine_counter = get_minefield_sum(field);

    /* Assert */
    assert(mine_counter == amount);
}

void test_get_biggest_cleared_zone() {
    /* Arrange */
    //Variables for Act
    int width = 5;
    int height = 5;
    minefield field = get_empty_minefield(width, height);
    int assert_zone_area;
    int test_zone_area;

    zone assert_zone =  {{1, 0}, {4, 2}};
    zone test_zone;

    // Place mines at specific positions for act test
    field.matrix[0][0].mine = 1;
    field.matrix[3][1].mine = 1;
    field.matrix[3][3].mine = 1;
    field.matrix[4][4].mine = 1;

    /* Act */
    test_zone = get_biggest_cleared_zone(field);

    test_zone_area = get_zone_area(test_zone);
    assert_zone_area = get_zone_area(assert_zone);

    /* Assert */
    assert(assert_zone_area == test_zone_area);
}


void test_get_biggest_clearable_zone() {
    printf("Testing get_biggest_clearable_zone:\n\n");

    /* Arrange */
    //Variables for Act
    int width = 5;
    int height = 5;
    minefield field = get_empty_minefield(width, height);
    int mine_removal_capacity = 1;
    int assert_zone_area;
    int test_zone_area;

    zone assert_zone =  {{0, 0}, {4, 2}};
    zone test_zone;

    // Place mines at specific positions for act test
    field.matrix[0][0].mine = 1;
    field.matrix[3][1].mine = 1;
    field.matrix[3][3].mine = 1;
    field.matrix[4][4].mine = 1;

    printf("Minefield to test:\n");
    print_minefield(field);

    printf("Predicted zone if one mine can be cleared:\n");
    print_minefield_zone(field, assert_zone);

    /* Act */
    test_zone = get_biggest_clearable_zone(field, mine_removal_capacity);

    test_zone_area = get_zone_area(test_zone);
    assert_zone_area = get_zone_area(assert_zone);

    /* Assert */
    assert(assert_zone_area == test_zone_area);

    printf("Test get_biggest_clearable_zone success!\n");
}


void test_binary_zoning() {

}