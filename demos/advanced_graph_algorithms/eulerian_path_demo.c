#include "advanced_graph_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

void eulerian_path_demo(void)
{
    int vertices, edges;
    Graph* graph = NULL;

    printf("\nEulerian Path & Circuit Demo (Directed Graph)\n");
    int input_status = safe_input_int(&vertices, "Enter number of vertices (2-20): ", 2, 20);
    if (input_status == INPUT_EXIT_SIGNAL)
        return;

    graph = create_graph(vertices);
    if (graph == NULL)
    {
        printf("\nError: Memory allocation failed for graph.\n");
        return;
    }
    int edge_status = safe_input_int(&edges, "Enter number of edges (1-100): ", 1, 100);
    if (edge_status == INPUT_EXIT_SIGNAL)
    {
        free_graph(graph);
        return;
    }

    printf("Enter directed edges (src dest) with vertices from 0 to %d:\n", vertices - 1);
    for (int i = 0; i < edges; i++)
    {
        int u, v;
        if (safe_input_int(&u, "src: ", 0, vertices - 1) == INPUT_EXIT_SIGNAL)
        {
            free_graph(graph);
            return;
        }
        if (safe_input_int(&v, "dest: ", 0, vertices - 1) == INPUT_EXIT_SIGNAL)
        {
            free_graph(graph);
            return;
        }
        add_edge_directed_unweighted(graph, u, v);
    }

    int path_len = 0;
    int* path = NULL;
    int res = find_eulerian_path(graph, &path, &path_len);

    if (res == 1 && path_len > 0)
    {
        printf("\nEulerian Path/Circuit Found:\n");
        for (int i = 0; i < path_len; i++)
        {
            printf("%d", path[i]);
            if (i < path_len - 1)
                printf(" -> ");
        }
        printf("\n");
        free(path);
    }
    else
    {
        printf("\nNo Eulerian Path or Circuit exists for the given graph.\n");
    }

    free_graph(graph);
    printf("\nPress Enter to continue...");
    getchar();
}
