#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include "minefield_algorithms.h"

void test_get_empty_minefield();
void test_get_random_minefield();
void test_get_zone_mine_sum();
void test_get_zone_area();

int main(void) {
    // Seed random generator
    srand(time(NULL));

    test_get_empty_minefield();
    test_get_random_minefield();
    test_get_zone_mine_sum();
    test_get_zone_area();

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
    // "== 0" to check that no mine is in [x][y] position
    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            assert(field.matrix[y][x].mine == 0);
        }
    }

    printf("Function 'get_empty_minefield' Passed!\n");
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

    printf("Function 'get_random_minefield' Passed!\n");
}

void test_get_zone_mine_sum() {
    /* Arrange */
    //Variables for Act
    int width = 5;
    int height = 5;
    minefield field;

// Initialize the minefield matrix with no mines
    field.height = height;
    field.width = width;
    for (int y = 0; y < field.height; ++y) {
        for (int x = 0; x < field.width; ++x) {
            field.matrix[y][x].mine = 0;
        }
    }

    // Place mines at specific positions
    field.matrix[1][2].mine = 1;
    field.matrix[2][3].mine = 1;
    field.matrix[3][1].mine = 1;
    field.matrix[3][3].mine = 1;
    field.matrix[4][4].mine = 1;

    zone biggest_rect;
    biggest_rect.start.x = 1;
    biggest_rect.start.y = 1;
    biggest_rect.end.x = 4;
    biggest_rect.end.y = 4;

    /* Act */
    int result = get_zone_mine_sum(field, biggest_rect);

    /* Assert */
    assert(result == 5);  // Adjust the expected value based on your minefield matrix and zone

    printf("Function 'get_zone_mine_sum' Passed!\n");
}


void test_get_zone_area() {
    /* Arrange */
    //Variables for Act
    zone testZone;
    testZone.start.x = 1;
    testZone.start.y = 1;
    testZone.end.x = 4;
    testZone.end.y = 4;

    /* Act */
    int result = get_zone_area(testZone);

    /* Assert */
    //Calculate the area manually to check if function calculates it correctly. (hvorfor +1 hvis det alligevel giver 4?)
    assert(result == 16);

    printf("Function 'get_zone_area' Passed!\n");
}