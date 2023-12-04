#include <stdio.h>
#include <math.h>
#include <time.h>
#include "experiments.h"
#include "../minefield_algorithms/minefield_algorithms.h"

double percent_error(double expected, double actual);

void run_comparison_experiment(int width, int height, int mine_count) {
    int mine_removal_capacity = mine_count-1;
    FILE *fp;
    int n = 1;

    while (1) {
        char filename[100];
        sprintf(filename, "experiments_output/log%d.txt", n);
        fp = fopen(filename, "r");
        if (fp == NULL) {
            break;
        }
        ++n;
        fclose(fp);
    }

    // make new log file
    char filename[100];
    sprintf(filename, "experiments_output/log%d.txt", n);
    fp = fopen(filename, "w");

    // write clocks per second
    fprintf(fp, "CLOCKS_PER_SEC: %d\n", CLOCKS_PER_SEC);

    // write header names
    fprintf(fp, "remaining_mines,Area,Clocks,Area percentage,Clocks\n");

    minefield field = get_random_minefield(width, height, mine_count);
    print_minefield(field);

    for (int current_capacity = 1; current_capacity <= mine_removal_capacity; ++current_capacity) {
        printf("current_capacity: %d\n", current_capacity);

        printf("begin get_biggest_clearable_zone\n");
        int start_clocks1 = clock();
        zone biggest_clearable_zone1 = get_biggest_clearable_zone(field, current_capacity);
        int area1 = get_zone_area(biggest_clearable_zone1);
        int elapsed_clocks1 = clock() - start_clocks1;
        printf("end get_biggest_clearable_zone\n");

        printf("beign quick clear\n");
        int start_clocks_2 = clock();
        zone biggest_clearable_zone2 = quick_clear(field, current_capacity);
        int area2 = get_zone_area(biggest_clearable_zone2);
        int elapsed_clocks2 = clock() - start_clocks_2;
        printf("end quick clear\n");

        fprintf(fp, "%d,%d,%d,%f,%d\n",
                mine_count-current_capacity,
                area1,
                elapsed_clocks1,
                (double) percent_error(area1, area2),
                elapsed_clocks2);
    }
}

double percent_error(double expected, double actual) {
    /* Gets percentage error */
    double error_rate = 100-(((fabs(actual-expected))/expected)*100);
    return error_rate;
}
