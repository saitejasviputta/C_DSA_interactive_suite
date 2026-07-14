#include "backtracking.h"
#include "config.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <time.h>

void graph_coloring_demo(void)
{

    // Predefined Graph Topologies
    GraphTopology topologies[] = {
        {"Cycle Graph C5",
         5,
         {{0, 1, 0, 0, 1}, {1, 0, 1, 0, 0}, {0, 1, 0, 1, 0}, {0, 0, 1, 0, 1}, {1, 0, 0, 1, 0}}},
        {"Bipartite Graph K3,3",
         6,
         {{0, 0, 0, 1, 1, 1},
          {0, 0, 0, 1, 1, 1},
          {0, 0, 0, 1, 1, 1},
          {1, 1, 1, 0, 0, 0},
          {1, 1, 1, 0, 0, 0},
          {1, 1, 1, 0, 0, 0}}},
        {"Complete Graph K4", 4, {{0, 1, 1, 1}, {1, 0, 1, 1}, {1, 1, 0, 1}, {1, 1, 1, 0}}},
        {"Complete Graph K5",
         5,
         {{0, 1, 1, 1, 1}, {1, 0, 1, 1, 1}, {1, 1, 0, 1, 1}, {1, 1, 1, 0, 1}, {1, 1, 1, 1, 0}}},
        {"Wheel Graph W5",
         5,
         {{0, 1, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 0, 1, 0}, {1, 0, 1, 0, 1}, {1, 1, 0, 1, 0}}},
        {"Petersen Graph",
         10,
         {{0, 1, 0, 0, 1, 1, 0, 0, 0, 0},
          {1, 0, 1, 0, 0, 0, 1, 0, 0, 0},
          {0, 1, 0, 1, 0, 0, 0, 1, 0, 0},
          {0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
          {1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 1, 1, 0},
          {0, 1, 0, 0, 0, 0, 0, 0, 1, 1},
          {0, 0, 1, 0, 0, 1, 0, 0, 0, 1},
          {0, 0, 0, 1, 0, 1, 1, 0, 0, 0},
          {0, 0, 0, 0, 1, 0, 1, 1, 0, 0}}}};
    int num_topologies = sizeof(topologies) / sizeof(topologies[0]);

    while (1)
    {
        int graph_choice;
        printf("\n--- Graph Coloring Topology ---\n");
        for (int i = 0; i < num_topologies; i++)
        {
            printf("%d. %s (%d vertices)\n", i + 1, topologies[i].name, topologies[i].num_vertices);
        }
        int status = safe_input_int(&graph_choice, "Select topology (1-6), or -1 to exit: ", 1,
                                    num_topologies);

        if (status == INPUT_EXIT_SIGNAL)
        {

            printf("\nReturning to Backtracking menu...\n");
            return;
        }

        if (status == 0)
            continue;

        const GraphTopology* selected_graph = &topologies[graph_choice - 1];

        int M;
        status = safe_input_int(
            &M, "\nEnter the number of colors M (between 1 and 5), or -1 to exit: ", 1, 5);
        if (status == INPUT_EXIT_SIGNAL)
        {

            printf("\nReturning to Backtracking menu...\n");
            return;
        }
        if (status == 0)
            continue;

        int delay_time = 150;
        int colors[MAX_V] = {0};

        print_graph_state(selected_graph, colors, 0, "Starting graph coloring...", delay_time);

        clock_t start_t = clock();
        int solved = solve_graph_coloring_util(selected_graph, M, colors, 0, delay_time);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        if (solved)
        {
            printf("\n🎉 SUCCESS: The graph was successfully colored using %d colors!\n", M);
        }
        else
        {
            printf("\n❌ FAILURE: No valid coloring exists for this graph with %d colors.\n", M);
        }
        printf("Time taken: %f seconds\n", total_t);
    }
}
