#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "experiments.h"
#include "../minefield_algorithms/minefield_algorithms.h"

double percent_error(double expected, double actual);
FILE* get_new_log_file(char* folder, int final);
char* get_new_log_folder();

void mulitiple_experiment_runs(int width, int height, int mine_count, int runs) {
    char* folder = get_new_log_folder();

    for (int i = 0; i < runs; ++i) {
        printf("Doing run_%d\n", i);
        experiment_run(width, height, mine_count, folder);
    }
}

void experiment_run(int width, int height, int mine_count, char* folder) {
    int mine_removal_capacity = mine_count-1;
    FILE *fp = get_new_log_file(folder, 0);

    // write header names
    fprintf(fp, "remaining_mines, Area, Clocks, Area percentage, Clocks, Area percentage, Clocks\n");

    minefield field = get_random_minefield(width, height, mine_count);
    print_minefield(field);

    int start_experiment_clocks = clock();
    for (int current_capacity = 1; current_capacity <= mine_removal_capacity; ++current_capacity) {
        int start_clocks = clock();
        zone biggest_clearable_zone = get_biggest_clearable_zone(field, current_capacity);
        int correct_area = get_zone_area(biggest_clearable_zone);
        int elapsed_clocks = clock() - start_clocks;

        fprintf(fp, "%d,%d,%d,",
            mine_count-current_capacity,
            correct_area,
            elapsed_clocks
        );

        start_clocks = clock();
        biggest_clearable_zone = quick_clear(field, current_capacity);
        int new_area = get_zone_area(biggest_clearable_zone);
        elapsed_clocks = clock() - start_clocks;

        fprintf(fp, "%f,%d,",
            percent_error(correct_area, new_area),
            elapsed_clocks
        );

        start_clocks = clock();
        biggest_clearable_zone = center_expansion(field, current_capacity);
        new_area = get_zone_area(biggest_clearable_zone);
        elapsed_clocks = clock() - start_clocks;


        fprintf(fp, "%f,%d",
            percent_error(correct_area, new_area),
            elapsed_clocks
        );

        fprintf(fp, "\n");
    }
    int end_experiment_clocks = clock();
    int time_elapsed = (end_experiment_clocks - start_experiment_clocks) / CLOCKS_PER_SEC;

    // write clocks per second
    fprintf(fp, "CLOCKS_PER_SEC: %d,\n", CLOCKS_PER_SEC);
    fprintf(fp, "total_seconds_elapsed: %d,\n", time_elapsed);
    fclose(fp);
}

double percent_error(double expected, double actual) {
    double error_rate = 100-(((fabs(actual-expected))/expected)*100);
    return error_rate;
}

char* get_new_log_folder() {
    int n = 1;
    char* foldername;

    while (1) {
        foldername = malloc(sizeof(char) * 100);
        sprintf(foldername, "experiments_output/experiment_%d", n);
        struct stat st = {0};
        if (stat(foldername, &st) == -1) {
            break;
        }
        ++n;
    }

    sprintf(foldername, "experiments_output/experiment_%d", n);
    mkdir(foldername);

    return foldername;
}

FILE* get_new_log_file(char* folder, int final) {
    FILE *fp;
    int n = 1;

    while (1) {
        char filename[100];
        if (final) {
            sprintf(filename, "%s/final.csv", folder);
        } else {
            sprintf(filename, "%s/run_%d.csv", folder, n);
        }
        fp = fopen(filename, "r");
        if (fp == NULL) {
            break;
        }
        ++n;
        fclose(fp);
    }

    char filename[100];

    if (final) {
        sprintf(filename, "%s/final.csv", folder);
    } else {
        sprintf(filename, "%s/run_%d.csv", folder, n);
    }

    fp = fopen(filename, "w");

    return fp;
}
