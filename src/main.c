#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "experiments/experiments.h"
#include "minefield_algorithms/minefield_algorithms.h"

int main(void) {
    // begin experiment
    srand(time(NULL));

    mulitiple_experiment_runs(5, 5, 20, 1);

    return EXIT_SUCCESS;
}
