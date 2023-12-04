#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "experiments/experiments.h"
#include "minefield_algorithms/minefield_algorithms.h"

int main(void) {
    // let user choose between running demo or experiments
    char choice;
    do {
        printf("Run demo or experiments? (d/e): \n");
        scanf("%c", &choice);
    } while (choice != 'd' && choice != 'e');

    if (choice == 'd') {
        // run demo
        printf("Run demo\n");
    } else {
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

        mulitiple_experiment_runs(width, height, mine_count, runs);
    }

    return EXIT_SUCCESS;
}
