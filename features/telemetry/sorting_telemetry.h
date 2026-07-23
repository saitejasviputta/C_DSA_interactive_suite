#ifndef SORTING_TELEMETRY_H
#define SORTING_TELEMETRY_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/time.h>

typedef struct
{
    const char* algorithm_name;
    long long comparisons;
    long long swaps;
    long long copies;
    int max_recursion_depth;
    int current_recursion_depth;
    int pass_count;
    double elapsed_microseconds;
    struct timeval start_time;
    struct timeval end_time;
    bool is_active;
} SortingTelemetry;

void sorting_telemetry_init(SortingTelemetry* t, const char* algorithm_name);
void sorting_telemetry_start(SortingTelemetry* t);
void sorting_telemetry_stop(SortingTelemetry* t);
void sorting_telemetry_reset(SortingTelemetry* t);

void sorting_telemetry_add_comparison(SortingTelemetry* t, long long count);
void sorting_telemetry_add_swap(SortingTelemetry* t, long long count);
void sorting_telemetry_add_copy(SortingTelemetry* t, long long count);
void sorting_telemetry_enter_recursion(SortingTelemetry* t);
void sorting_telemetry_exit_recursion(SortingTelemetry* t);
void sorting_telemetry_add_pass(SortingTelemetry* t, int count);

void sorting_telemetry_print_summary(const SortingTelemetry* t);
void sorting_telemetry_dashboard_demo(void);

#endif // SORTING_TELEMETRY_H
