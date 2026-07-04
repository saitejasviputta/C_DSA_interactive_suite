#define _GNU_SOURCE
#include "benchmark.h"
#include "graph_traversals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#else
#include <unistd.h>
#endif

void run_graphs_benchmark(int v)
{
    // Seed random generator with fixed seed
    srand(BENCHMARK_SEED);

    // Safety Guard: bypass all graph benchmarks if V is too large to prevent freezes
    if (v > 2000)
    {
        printf("\nGraph Shortest Path benchmark skipped for V = %d (Threshold is V = 2000)\n", v);
        return;
    }

    // Allocate graph
    weightedGraph* graph = create_weightedGraph(v);
    if (graph == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for graph V = %d\n", v);
        return;
    }

    // Ensure connectedness by creating a chain 0 -> 1 -> ... -> V-1
    for (int i = 0; i < v - 1; i++)
    {
        add_edge_directed(graph, i, i + 1, rand() % 90 + 10);
    }

    // Add additional random edges (5% probability)
    for (int src = 0; src < v; src++)
    {
        for (int dest = 0; dest < v; dest++)
        {
            if (src != dest)
            {
                if (rand() % 100 < 5)
                {
                    add_edge_directed(graph, src, dest, rand() % 90 + 10);
                }
            }
        }
    }

    // Generate heuristics array for A* and Greedy Best-First Search
    int* h = malloc(v * sizeof(int));
    if (h == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for heuristics array\n");
        free_weightedGraph(graph);
        return;
    }
    for (int i = 0; i < v; i++)
    {
        h[i] = (v - 1 - i) * 5;
    }

    // Print table header
    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: GRAPH SHORTEST PATH (V = %d)\n", v);
    printf("========================================================================\n");
    printf("%-30s %-25s %-15s %-10s\n", "Algorithm", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    const char* algos[] = {"Dijkstra", "Bellman-Ford", "A* Search", "Greedy Best-First Search"};

    int start = 0;
    int dest = v - 1;

    for (int i = 0; i < 4; i++)
    {
        const char* name = algos[i];
        int skip = 0;

        // Skip Bellman-Ford (O(V * E)) for V > 500
        if (v > 500 && i == 1)
        {
            skip = 1;
        }

        if (skip)
        {
            printf("%-30s %-25s %-15s %-10s\n", name, "Skipped (V > 500)", "N/A", "SKIPPED");
            continue;
        }

        double times[BENCHMARK_DEFAULT_ITERATIONS];
        size_t peak_mem = 0;

        RUN_BENCHMARK(times, peak_mem, {
            // Redirect stdout to suppress prints from algorithm steps
            fflush(stdout);
            int stdout_dup = dup(1);
#ifdef _WIN32
            FILE* fnull = fopen("NUL", "w");
#else
            FILE* fnull = fopen("/dev/null", "w");
#endif
            if (fnull != NULL && stdout_dup >= 0)
            {
                dup2(fileno(fnull), 1);
            }

            // Run algorithm
            switch (i)
            {
                case 0:
                    dijkstra(graph, start);
                    break;
                case 1:
                    bellman_ford(graph, start);
                    break;
                case 2:
                    astar(graph, start, dest, h);
                    break;
                case 3:
                    greedy_best_first_search(graph, start, dest, h);
                    break;
            }

            // Restore stdout
            fflush(stdout);
            if (stdout_dup >= 0)
            {
                dup2(stdout_dup, 1);
                close(stdout_dup);
            }
            if (fnull != NULL)
            {
                fclose(fnull);
            }
        });

        // Print row and export
        benchmark_report_result("graphs", name, v, times, peak_mem);
    }

    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/graphs.csv'.\n");

    free(h);
    free_weightedGraph(graph);
}
