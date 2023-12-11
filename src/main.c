#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "experiments/experiments.h"
#include "minefield_algorithms/minefield_algorithms.h"

int main(void) {
    // set random seed
    srand(time(NULL));

    // run experiments
    int width, height, mine_count, runs, logging, interval, with_brute_force, with_gradiant_minefield;
    printf("Enter width:\n");
    scanf("%d", &width);
    printf("Enter height:\n");
    scanf("%d", &height);
    printf("Enter mine count:\n");
    scanf("%d", &mine_count);
    printf("Enter number of runs:\n");
    scanf("%d", &runs);
    printf("Enter logging (0 or 1):\n");
    scanf("%d", &logging);
    printf("Enter mine removal capacity interval:\n");
    scanf("%d", &interval);
    printf("Enter with_brute_force (0 or 1):\n");
    scanf("%d", &with_brute_force);
    printf("Enter with_gradiant_minefield (0 or 1):\n");
    scanf("%d", &with_gradiant_minefield);

    multiple_experiment_runs(width, height, mine_count, runs, with_brute_force, with_gradiant_minefield, logging, interval);

    return EXIT_SUCCESS;
}
