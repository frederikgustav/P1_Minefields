#include <stdio.h>
#include <math.h>
#include <time.h>
#include "experiments.h"

double begin_timestamp();
double end_timestamp(double start_time);
double percent_error(double expected, double actual);

void run_experiment(int width, int height, int mine_count, int mine_removal_capacity) {
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

    // write header names
    fprintf(fp, "remaining_mines,Area,Time,Area percentage,Time\n");

    minefield field = get_random_minefield(width, height, mine_count);

    for (int current_capacity = 1; current_capacity <= mine_removal_capacity; ++current_capacity) {
        printf("current_capacity: %d\n", current_capacity);
        double start_time1 = begin_timestamp();
        zone biggest_clearable_zone1 = get_biggest_clearable_zone(field, current_capacity);
        int area1 = get_zone_area(biggest_clearable_zone1);
        double end_time1 = end_timestamp(start_time1);

        double start_time2 = begin_timestamp();
        zone biggest_clearable_zone2 = quick_clear(field, current_capacity);
        int area2 = get_zone_area(biggest_clearable_zone2);
        double end_time2 = end_timestamp(start_time2);

        // the area percentage is how big area2 is compared to area

        fprintf(fp, "%d,%d,%f,%f,%f\n", mine_count-current_capacity, area1, end_time1, (double) percent_error(area1, area2), end_time2);

    }

}

double begin_timestamp() {
    return time(NULL);
};

double end_timestamp(double start_time) {
    return time(NULL) - start_time;
};

double percent_error(double expected, double actual) {
    /* Gets percentage error */
    double error_rate = 100-((fabs(actual-expected))/expected)*100;
    return error_rate;
}
