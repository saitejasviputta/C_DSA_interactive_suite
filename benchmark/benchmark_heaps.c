#include "advanced_heaps.h"
#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void run_heaps_benchmark(int n)
{
    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: ADVANCED HEAPS (N = %d)\n", n);
    printf("========================================================================\n");
    printf("%-30s %-25s %-15s %-10s\n", "Heap Type", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    // Initialize random number generator with fixed seed
    srand(BENCHMARK_SEED);
    int* random_keys = (int*)malloc(n * sizeof(int));
    int* random_vals = (int*)malloc(n * sizeof(int));
    if (random_keys == NULL || random_vals == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for benchmark keys/values\n");
        free(random_keys);
        free(random_vals);
        return;
    }
    for (int i = 0; i < n; i++)
    {
        random_keys[i] = rand() % 1000000;
        random_vals[i] = i;
    }

    // 1. Binomial Heap Benchmark
    {
        double times[BENCHMARK_DEFAULT_ITERATIONS];
        size_t peak_mem = 0;

        RUN_BENCHMARK(times, peak_mem, {
            BinomialNode* heap = NULL;

            // Perform inserts
            for (int i = 0; i < n; i++)
            {
                heap = binomial_heap_insert(heap, random_keys[i], random_vals[i]);
            }

            // Perform extracts
            for (int i = 0; i < n; i++)
            {
                int k, v;
                heap = binomial_heap_extract_min(heap, &k, &v);
            }
            destroy_binomial_heap(heap);
        });

        benchmark_report_result("heaps", "Binomial Heap", n, times, peak_mem);
    }

    // 2. Fibonacci Heap Benchmark
    {
        double times[BENCHMARK_DEFAULT_ITERATIONS];
        size_t peak_mem = 0;

        RUN_BENCHMARK(times, peak_mem, {
            FibonacciNode* heap = NULL;

            // Perform inserts
            for (int i = 0; i < n; i++)
            {
                heap = fib_heap_insert(heap, random_keys[i], random_vals[i]);
            }

            // Perform extracts
            for (int i = 0; i < n; i++)
            {
                int k, v;
                heap = fib_heap_extract_min(heap, &k, &v);
            }
            destroy_fibonacci_heap(heap);
        });

        benchmark_report_result("heaps", "Fibonacci Heap", n, times, peak_mem);
    }

    // 3. Min-Max Heap Benchmark
    {
        double times[BENCHMARK_DEFAULT_ITERATIONS];
        size_t peak_mem = 0;

        RUN_BENCHMARK(times, peak_mem, {
            MinMaxHeap* heap = create_min_max_heap(n);
            if (heap != NULL)
            {
                // Perform inserts
                for (int i = 0; i < n; i++)
                {
                    min_max_heap_insert(heap, random_keys[i], random_vals[i]);
                }

                // Perform extracts
                for (int i = 0; i < n; i++)
                {
                    int k, v;
                    min_max_heap_extract_min(heap, &k, &v);
                }
                destroy_min_max_heap(heap);
            }
        });

        benchmark_report_result("heaps", "Min-Max Heap", n, times, peak_mem);
    }

    free(random_keys);
    free(random_vals);

    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/heaps.csv'.\n");
}
