#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "experiments/experiments.h"
#include "minefield_algorithms/minefield_algorithms.h"

int main(void) {
    // set random seed
    srand(time(NULL));

    // run experiments
    int width, height, mine_count, runs;
    printf("Enter width:\n");
    scanf("%d", &width);
    printf("Enter height:\n");
    scanf("%d", &height);
    printf("Enter mine count:\n");
    scanf("%d", &mine_count);
    printf("Enter number of runs:\n");
    scanf("%d", &runs);

    multiple_experiment_runs(width, height, mine_count, runs, 0, 1, 1);

    return EXIT_SUCCESS;
}
