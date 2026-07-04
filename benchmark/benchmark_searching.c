#include "benchmark.h"
#include "searching_algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void run_searching_benchmark(int n)
{
    // Seed random generator with fixed seed
    srand(BENCHMARK_SEED);

    // Allocate sorted array
    int* arr = malloc(n * sizeof(int));
    if (arr == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for benchmark size N = %d\n", n);
        return;
    }

    // Initialize sorted values with uniform distribution (ideal for Interpolation Search)
    arr[0] = rand() % 5 + 1;
    for (int i = 1; i < n; i++)
    {
        arr[i] = arr[i - 1] + (rand() % 5 + 1);
    }

    // Pre-generate 10,000 query targets to search for (ensuring stable profiling)
    int num_queries = 10000;
    int* queries = malloc(num_queries * sizeof(int));
    if (queries == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for benchmark queries\n");
        free(arr);
        return;
    }
    for (int i = 0; i < num_queries; i++)
    {
        queries[i] = arr[rand() % n];
    }

    // Print table header
    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: SEARCHING ALGORITHMS (N = %d, Loops = %d)\n", n,
           num_queries);
    printf("========================================================================\n");
    printf("%-30s %-25s %-15s %-10s\n", "Algorithm", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    const char* algos[] = {"Linear Search", "Binary Search (Iterative)",
                           "Binary Search (Recursive)", "Jump Search", "Interpolation Search"};

    for (int i = 0; i < 5; i++)
    {
        const char* name = algos[i];
        int skip = 0;

        // Skip Linear Search (O(N)) for N > 10000 to prevent long freezes in TUI
        if (n > 10000 && i == 0)
        {
            skip = 1;
        }

        if (skip)
        {
            printf("%-30s %-25s %-15s %-10s\n", name, "Skipped (N > 10000)", "N/A", "SKIPPED");
            continue;
        }

        double times[BENCHMARK_DEFAULT_ITERATIONS];
        size_t peak_mem = 0;

        RUN_BENCHMARK(times, peak_mem, {
            volatile int dummy = 0;
            for (int q = 0; q < num_queries; q++)
            {
                int target = queries[q];
                int res = -1;
                switch (i)
                {
                    case 0:
                        res = linear_search(arr, target, n);
                        break;
                    case 1:
                        res = binary_search(arr, target, n);
                        break;
                    case 2:
                        res = binary_search_recursive(arr, target, 0, n - 1);
                        break;
                    case 3:
                        res = jump_search(arr, target, n);
                        break;
                    case 4:
                        res = interpolation_search(arr, target, n);
                        break;
                }
                dummy += res;
            }
            (void)dummy;
        });

        // Print row and export
        benchmark_report_result("searching", name, n, times, peak_mem);
    }

    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/searching.csv'.\n");

    free(queries);
    free(arr);
}
