#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "experiments.h"
#include "../minefield_algorithms/minefield_algorithms.h"

struct stat st = {0};

double percent_error(double expected, double actual);
FILE* get_new_log_file(char* folder, int final);
char* get_new_log_folder();

void mulitiple_experiment_runs(int width, int height, int mine_count, int runs) {
    char* folder = get_new_log_folder();

    printf("Doing runs: ");
    for (int i = 1; i <= runs; ++i) {
        printf("%d, ", i);
        experiment_run(width, height, mine_count, folder);
    }

    // print full path to folder
    printf("\nSaved results of experiment in folder: %s\n", folder);

    free(folder);
}

void experiment_run(int width, int height, int mine_count, char* folder) {
    int mine_removal_capacity = mine_count-1;
    FILE *fp = get_new_log_file(folder, 0);

    // write header names
    fprintf(fp, "General info, Bruteforce, , Quick Clear, , Center expansion, \n");
    fprintf(fp, "Remaining mines, Area, Clocks, Area percentage, Clocks, Area percentage, Clocks \n");

    minefield field = get_random_minefield(width, height, mine_count);

    int start_experiment_clocks = clock();
    for (int current_capacity = 1; current_capacity <= mine_removal_capacity; ++current_capacity) {
        int start_clocks = clock();
        zone biggest_clearable_zone = get_biggest_clearable_zone(field, current_capacity);
        int correct_area = get_zone_area(biggest_clearable_zone);
        int elapsed_clocks = clock() - start_clocks;

        fprintf(fp, "%d, %d,%d, ",
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
    fprintf(fp, "start_mine_count: %d,\n", mine_count);
    fclose(fp);
    free_minefield(field);
}

double percent_error(double expected, double actual) {
    double error_rate = 100-(((fabs(actual-expected))/expected)*100);
    return error_rate;
}

char* get_new_log_folder() {
    // make experiments_output folder if it doesn't exist
    if (stat("experiments_output", &st) == -1) {
        int made_folder = mkdir("experiments_output");
        if (made_folder != 0) {
            printf("Could not make folder experiments_output\n");
            exit(EXIT_FAILURE);
        }
    }

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
    int made_folder = mkdir(foldername);
    if (made_folder != 0) {
        printf("Could not make folder %s\n", foldername);
        exit(EXIT_FAILURE);
    }

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

    if (fp == NULL) {
        printf("Could not open or make file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    return fp;
}
