#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "../src/minefield_algorithms/minefield_algorithms.h"
#define TOLERANCE 0.0001

void test_get_empty_minefield();
void test_get_random_minefield();
void test_get_zone_mine_sum();
void test_get_zone_area();
void test_get_minefield_sum();
void test_get_biggest_cleared_zone();
void test_get_biggest_clearable_zone();
void test_binary_zoning_case_1();
void test_binary_zoning_case_2();
void test_is_valid_zone_case_1();
void test_is_valid_zone_case_2();
void test_get_zone_height();
void test_get_zone_width();
void test_get_zone_mine_density();

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
    test_binary_zoning_case_1();
    test_binary_zoning_case_2();
    test_is_valid_zone_case_1();
    test_is_valid_zone_case_2();
    test_get_zone_height();
    test_get_zone_width();
    test_get_zone_mine_density();

    return EXIT_SUCCESS;
}

/**
 * Confirms there aren't any mines in a given empty minefield
 */
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
/**
 * Tests that the amount of mines in a random minefield is correct
 */
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

/**
 * Tests that function that counts the sum of mines in zone of a minefield is correct
 */
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

/**
 * Test othat function that gets the area of zone in minefield is correct
 */
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

/**
 * Tests that function that gets the sum of mines in a whole minefield is correct
 */
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

/**
 * Confirms that the zone with the biggest cleared area is found
 */
void test_get_biggest_cleared_zone() {
    printf("Testing get_biggest_cleared_zone:\n");
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

    printf("Minefield to test:\n");
    print_minefield(field);

    printf("Predicted zone:\n");
    print_minefield_zone(field, assert_zone);

    /* Act */
    test_zone = get_biggest_cleared_zone(field);

    test_zone_area = get_zone_area(test_zone);
    assert_zone_area = get_zone_area(assert_zone);

    /* Assert */
    assert(assert_zone_area == test_zone_area);
    printf("Test get_biggest_cleared_zone: success!\n\n");
}

/**
 * Test of function that gives a zone in minefield that will give biggest cleared when clearing x amount of mines
 */
void test_get_biggest_clearable_zone() {
    printf("Testing get_biggest_clearable_zone:\n");

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

    printf("Test get_biggest_clearable_zone: success!\n\n");
}
/**
 * Test of function that uses binary algorithm on minefield to create zone that is approximately
 * the best start zone for clearing mines
 */
void test_binary_zoning_case_1() {
    printf("Testing binary_zoning case 1:\n");

    /* Arrange */
    //Variables for Act
    int width = 5;
    int height = 5;
    minefield field = get_empty_minefield(width, height);
    int mine_removal_capacity = 1;
    int assert_zone_area;
    int test_zone_area;

    zone assert_zone =  {{0, 0}, {2, 2}};
    zone test_zone;

    // Place mines at specific positions for act test
    field.matrix[0][0].mine = 1;
    field.matrix[3][1].mine = 1;
    field.matrix[3][3].mine = 1;
    field.matrix[4][4].mine = 1;

    printf("Minefield to test:\n");
    print_minefield(field);

    printf("Predicted zone if 1 mine can be cleared:\n");
    print_minefield_zone(field, assert_zone);

    /* Act */
    test_zone = binary_zoning(field, mine_removal_capacity);

    printf("Test binary_zoning case 1: success!\n\n");

    test_zone_area = get_zone_area(test_zone);
    assert_zone_area = get_zone_area(assert_zone);



    /* Assert */
    assert(assert_zone_area == test_zone_area);
}

void test_binary_zoning_case_2() {
    printf("Testing binary_zoning case 2:\n");

    /* Arrange */
    //Variables for Act
    int width = 10;
    int height = 10;
    minefield field = get_empty_minefield(width, height);
    int mine_removal_capacity = 2;
    int assert_zone_area;
    int test_zone_area;

    zone assert_zone =  {{5, 0}, {9, 9}};
    zone test_zone;

    // Place mines at specific positions for act test
    field.matrix[2][3].mine = 1;
    field.matrix[4][4].mine = 1;
    field.matrix[4][9].mine = 1;
    field.matrix[6][2].mine = 1;
    field.matrix[6][3].mine = 1;
    field.matrix[8][1].mine = 1;
    field.matrix[8][2].mine = 1;
    field.matrix[8][6].mine = 1;

    printf("Minefield to test:\n");
    print_minefield(field);

    printf("Predicted zone if 2 mines can be cleared:\n");
    print_minefield_zone(field, assert_zone);

    /* Act */
    test_zone = binary_zoning(field, mine_removal_capacity);

    test_zone_area = get_zone_area(test_zone);
    assert_zone_area = get_zone_area(assert_zone);

    /* Assert */
    assert(assert_zone_area == test_zone_area);

    printf("Test binary_zoning case 2: success!\n\n");
}

/**
 * Test of function that checks if the zone is valid (not out of bounds)
 */
void test_is_valid_zone_case_1() {
    /* Arrange */
    //Variables for Act
    int width = 5;
    int height = 5;
    minefield field = get_empty_minefield(width, height);
    zone test_zone = {{1, 1}, {4, 4}};

    /* Act */
    int result = is_valid_zone(field, test_zone);

    /* Assert */
    assert(result); // Zone is valid
}

void test_is_valid_zone_case_2() {
    /* Arrange */
    //Variables for Act
    int width = 5;
    int height = 5;
    minefield field = get_empty_minefield(width, height);
    zone test_zone = {{-1, -1}, {4, 4}};

    /* Act */
    int result = is_valid_zone(field, test_zone);

    /* Assert */
    assert(!result); // Zone is not valid
}

/**
 * Test of function that gets the height of a zone
 */
void test_get_zone_height() {

    /* Arrange */
    zone test_zone = {{1, 2}, {3, 5}};

    /* Act */
    int result = get_zone_height(test_zone);

    /* Assert */
    assert(result == 4);
}

/**
 * Test of function that gets the width of a zone
 */
void test_get_zone_width() {

    /* Arrange */
    zone test_zone = {{1, 2}, {3, 5}};

    /* Act */
    int result = get_zone_width(test_zone);

    /* Assert */
    assert(result == 3);
}

/**
 * Test of function that gets the density of mines in a zone
 */
void test_get_zone_mine_density() {
    /* Arrange */
    minefield field = get_empty_minefield(3, 3);
    field.matrix[0][0].mine = 1;
    field.matrix[1][1].mine = 1;
    field.matrix[2][2].mine = 1;

    zone test_zone = {{0, 0}, {2, 2}};

    /* Act */
    double result = get_zone_mine_density(field, test_zone);

    /* Assert */
    // In this case, there are 3 mines in a zone of size 3x3, so the expected density is 3/9 = 1/3
    double expected_density = 1.0 / 3.0;
    double diff = result - expected_density;

    // Compare the actual and expected results with a small tolerance
    assert(-TOLERANCE < diff && diff < TOLERANCE);
}
