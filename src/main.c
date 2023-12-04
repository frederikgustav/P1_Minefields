#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "experiments.h"

int main(void) {
    // begin experiment
    printf("begin experiment\n");
    run_experiment(5, 5, 4, 2);

    return EXIT_SUCCESS;
}
