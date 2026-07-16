#include "advanced_graph_algorithms.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

void advanced_graph_algorithms_demo(void)
{
    while (1)
    {
        display_header("Advanced Graph Algorithms");

        int choice;
        int status = safe_input_int(&choice,
                                    "\nAdvanced Graph Algorithms Demo\n"
                                    "------------------------------\n"
                                    "\n"
                                    "1. Tarjan's SCC\n"
                                    "2. Kosaraju's SCC\n"
                                    "3. Ford-Fulkerson Max Flow\n"
                                    "4. Edmonds-Karp Max Flow\n"
                                    "5. Dinic's Max Flow\n"
                                    "6. Dinic Bipartite Matching\n"
                                    "7. Hopcroft-Karp Bipartite Matching\n"
                                    "8. Eulerian Path\n"
                                    "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                    1, 8);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nReturning to main menu...\n");
            return;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                display_header("Strongly Connected Components (Tarjan)");
                tarjan_scc_demo();
                break;
            case 2:
                display_header("Strongly Connected Components (Kosaraju)");
                kosaraju_scc_demo();
                break;
            case 3:
                display_header("Maximum Flow (Ford-Fulkerson)");
                ford_fulkerson_demo();
                break;
            case 4:
                display_header("Maximum Flow (Edmonds-Karp)");
                edmonds_karp_demo();
                break;
            case 5:
                display_header("Maximum Flow (Dinic)");
                dinic_demo();
                break;
            case 6:
                display_header("Bipartite Matching (Dinic)");
                dinic_bipartite_matching_demo();
                break;
            case 7:
                display_header("Bipartite Matching (Hopcroft-Karp)");
                hopcroft_karp_demo();
                break;
            case 8:
                display_header("Eulerian Path");
                eulerian_path_demo();
                break;
        }
    }
}
