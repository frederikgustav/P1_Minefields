#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "experiments/experiments.h"
#include "minefield_algorithms/minefield_algorithms.h"
#include <windows.h>

void run_permutation_generation_demo();
void run_random_point_expansion_demo();
void run_binary_zoning_demo();

int main(void) {
    // set random seed
    srand(time(NULL));

    printf("Which demo would you like to run?\n");
    printf("1. Permutation generation\n");
    printf("2. Center expansion\n");
    printf("3. Binary zoning\n");
    printf("4. \n");
    printf("5. \n");

    int choice = 0;
    while (choice < 1 || choice > 5) {
        printf("Enter your choice: ");
        scanf("%d", &choice);
    }

    switch (choice) {
        case 1:
            run_permutation_generation_demo();
            break;
        case 2:
            run_random_point_expansion_demo();
            break;
        case 3:
            run_binary_zoning_demo();
            break;
        case 4:
            //run_demo_4();
            break;
        case 5:
            //run_demo_5();
            break;
        default:
            printf("Invalid choice\n");
            exit(EXIT_FAILURE);
    }


    return EXIT_SUCCESS;
}

void run_permutation_generation_demo() {
    printf("Using the following minefield:\n");
    minefield field = get_random_minefield(3, 3, 4);
    print_minefield(field);
    printf("Generated permutations:\n");
    zone best_zone;
    best_zone = get_biggest_clearable_zone(field, 2);
    printf("Best zone: \n");
    print_minefield_zone(field, best_zone);
}

void run_random_point_expansion_demo() {
    minefield field = get_random_minefield(30, 30, 100);

    printf("When starting from random point and removing 50 mines:\n");
    zone zone;
    zone = random_point_expansion(field, 50);
}

void run_binary_zoning_demo() {
    minefield field = get_random_minefield(30, 30, 100);

    zone zone;
    zone = binary_zoning(field, 10);
}