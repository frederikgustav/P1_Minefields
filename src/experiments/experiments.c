// experiments.c
// Implements functions for running experiments
// Beware! The file handling parts of this code are not original.

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "experiments.h"
#include "../minefield_algorithms/minefield_algorithms.h"

struct stat st = {0};

FILE* get_new_log_file(char* folder, int final);
FILE* experiment_run(int width, int height, int mine_count, char* folder, int with_brute_force, int logging, int interval);
char* get_new_log_folder();

void multiple_experiment_runs(int width, int height, int mine_count, int runs, int with_brute_force, int logging, int interval) {
    char* folder = get_new_log_folder();
    FILE* run_files[runs];

    int start_experiment_clocks = clock();
    if (logging) printf("Doing runs:\n");
    for (int i = 1; i <= runs; ++i) {
        if (logging) printf(" - Run %d/%d: \n", i, runs);
        int start_clocks = clock();
        run_files[i-1] = experiment_run(width, height, mine_count, folder, with_brute_force, logging, interval);
        int time_elapsed = (clock() - start_clocks) / CLOCKS_PER_SEC;
        if (logging) printf(" - Completed in a total of %d seconds.\n", time_elapsed);
    }
    if (logging) printf("Finished experiments.\n");
    if (logging) printf("Calculating averages: \n");

    // close files and open in read mode
    for (int run = 0; run < runs; ++run) {
        fclose(run_files[run]);
        char filename[100];
        sprintf(filename, "%s/run_%d.csv", folder, run+1);
        run_files[run] = fopen(filename, "r");
    }

    // write final file
    FILE* final_file = get_new_log_file(folder, 1);
    fprintf(final_file, "General info, Bruteforce, , Quick Clear, , Random point expansion, \n");
    fprintf(final_file, "Remaining mines, Area, Seconds, Area, Seconds, Area, Seconds \n");

    // calculate averages and write them to final file
    for (int current_capacity = 1; current_capacity <= mine_count; current_capacity = current_capacity + interval) {
        double total_correct_area = 0;
        double total_correct_seconds = 0;
        double total_new_area = 0;
        int total_new_seconds = 0;
        double total_new_area2 = 0;
        int total_new_seconds2 = 0;

        for (int i = 0; i < runs; ++i) {
            int correct_area, new_seconds;
            double new_area, new_area2, correct_seconds, new_seconds2;
            // scan values, throw away first value
            fscanf(run_files[i], "%*d, %d,%lf, %lf,%d, %lf,%lf\n",
                &correct_area,
                &correct_seconds,
                &new_area,
                &new_seconds,
                &new_area2,
                &new_seconds2
            );

            total_correct_area += correct_area;
            total_correct_seconds += correct_seconds;
            total_new_area += new_area;
            total_new_seconds += new_seconds;
            total_new_area2 += new_area2;
            total_new_seconds2 += new_seconds2;

        }

        // calculate averages
        double avg_correct_area = total_correct_area / runs;
        double avg_correct_clocks = (((double) total_correct_seconds / runs) / CLOCKS_PER_SEC);
        double avg_new_area = total_new_area / runs;
        double avg_new_seconds = (((double) total_new_seconds / runs) / CLOCKS_PER_SEC);
        double avg_new_area2 = total_new_area2 / runs;
        double avg_new_seconds2 = (((double) total_new_seconds2 / runs) / CLOCKS_PER_SEC);

        // print averages to final file
        fprintf(final_file, "%d, %f,%f, %f,%f, %f,%f\n",
            mine_count-current_capacity,
            avg_correct_area,
            avg_correct_clocks,
            avg_new_area,
            avg_new_seconds,
            avg_new_area2,
            avg_new_seconds2
        );
    }

    int end_experiment_time = (clock() - start_experiment_clocks) / CLOCKS_PER_SEC;

    if (logging) printf("Completed in a total of %d seconds.\n", end_experiment_time);

    // print general info
    fprintf(final_file, "total_seconds_elapsed: %d,\n", end_experiment_time);
    fprintf(final_file, "start_mine_count: %d,\n", mine_count);
    fprintf(final_file, "minefield_size: %dx%d,\n", width, height);
    // print number of runs
    fprintf(final_file, "number_of_runs: %d,\n", runs);

    // print full path to folder
    if (logging) printf("Finished and saved results of experiment in folder: %s\n", folder);

    free(folder);
}

FILE* experiment_run(int width, int height, int mine_count, char* folder, int with_brute_force, int logging, int interval) {
    int mine_removal_capacity = mine_count;
    FILE *fp = get_new_log_file(folder, 0);

    minefield field = get_random_minefield(width, height, mine_count);

    int start_experiment_clocks = clock();
    for (int current_capacity = 1; current_capacity <= mine_removal_capacity; current_capacity = current_capacity + interval) {
        int start_run_clocks = clock();
        if (logging) printf("    - Step %d: ", current_capacity);
        int start_clocks = clock();
        zone biggest_clearable_zone;
        if (!with_brute_force) {
            biggest_clearable_zone.end.x = 0;
            biggest_clearable_zone.end.y = 0;
            biggest_clearable_zone.start.x = 0;
            biggest_clearable_zone.start.y = 0;
        } else {
            biggest_clearable_zone = get_biggest_clearable_zone(field, current_capacity);
        }

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

        fprintf(fp, "%d,%d,",
            new_area,
            elapsed_clocks
        );

        start_clocks = clock();
        biggest_clearable_zone = random_point_expansion(field, current_capacity);
        new_area = get_zone_area(biggest_clearable_zone);
        elapsed_clocks = clock() - start_clocks;

        fprintf(fp, "%d,%d",
            new_area,
            elapsed_clocks
        );
        int total_run_time = (clock() - start_run_clocks) / CLOCKS_PER_SEC;
        if (logging) printf("Took %d seconds.\n", total_run_time);
        fprintf(fp, "\n");
    }
    int end_experiment_clocks = clock();
    int time_elapsed = (end_experiment_clocks - start_experiment_clocks) / CLOCKS_PER_SEC;

    // write clocks per second
    fprintf(fp, "total_seconds_elapsed: %d,\n", time_elapsed);
    fprintf(fp, "start_mine_count: %d,\n", mine_count);
    free_minefield(field);

    return fp;
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
