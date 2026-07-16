#include "advanced_graph_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Graph* get_scc_demo_graph(void)
{
    int edges;
    int graph_capacity;
    int input_method;
    Graph* graph = NULL;

    while (1)
    {
        int method_status = safe_input_int(&input_method,
                                           "\nenter 1 to build the graph manually, 2 to load it "
                                           "from a CSV file, enter '-1' to exit : ",
                                           1, 2);

        if (method_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting SCC visualization demo.....\n");
            return NULL;
        }

        if (method_status == 0)
        {
            continue;
        }

        break;
    }

    if (input_method == 2)
    {
        while (1)
        {
            char path[256];
            printf("\nenter the path to the CSV file, enter '-1' to exit : ");
            fflush(stdout);

            if (!fgets(path, sizeof(path), stdin))
            {
                printf("\ninput ended unexpectedly\n");
                clearerr(stdin);
                return NULL;
            }

            size_t len = strlen(path);
            while (len > 0 && (path[len - 1] == '\n' || path[len - 1] == '\r'))
                path[--len] = '\0';

            if (strcmp(path, "-1") == 0)
            {
                printf("\nExiting SCC visualization demo.....\n");
                return NULL;
            }

            if (len == 0)
            {
                continue;
            }

            graph = load_graph_from_csv(path);

            if (!graph)
            {
                continue;
            }

            break;
        }
        graph_capacity = graph->V;
    }
    else if (input_method == 1)
    {
        while (1)
        {
            int graph_capacity_status =
                safe_input_int(&graph_capacity,
                               "\nenter the number of vertices in the graph, "
                               "(between 1 and 10), enter '-1' to exit : ",
                               1, 10);

            if (graph_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting SCC visualization demo.....\n");
                return NULL;
            }

            if (graph_capacity_status == 0)
            {
                continue;
            }

            graph = create_graph(graph_capacity);
            if (!graph)
            {
                printf("\nmalloc allocation failed\n");
                return NULL;
            }

            break;
        }

        while (1)
        {
            int edges_capacity_status = safe_input_int(
                &edges,
                "\nenter number of directed edges (between 1 and 100), enter '-1' to exit :", 1,
                100);

            if (edges_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting SCC visualization demo\n");
                free_graph(graph);
                return NULL;
            }

            if (edges_capacity_status == 0)
            {
                continue;
            }

            break;
        }

        printf("\nenter directed edges (src dest) (vertices from 0 to %d, enter '-1' to exit):\n",
               graph_capacity - 1);

        for (int i = 0; i < edges; i++)
        {
            int src_status;
            int dest_status;
            int src;
            int dest;

        retry:
            src_status = safe_input_int(&src, "src: ", 0, graph_capacity - 1);

            if (src_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting SCC visualization demo\n");
                free_graph(graph);
                return NULL;
            }
            if (src_status == 0)
            {
                goto retry;
            }

            dest_status = safe_input_int(&dest, "dest: ", 0, graph_capacity - 1);

            if (dest_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting SCC visualization demo\n");
                free_graph(graph);
                return NULL;
            }
            if (dest_status == 0)
            {
                goto retry;
            }

            add_edge_directed_unweighted(graph, src, dest);
        }
    }
    return graph;
}

void tarjan_scc_demo(void)
{
    Graph* graph = get_scc_demo_graph();
    if (graph != NULL)
    {
        visualize_tarjan(graph);
        free_graph(graph);
    }
}

void kosaraju_scc_demo(void)
{
    Graph* graph = get_scc_demo_graph();
    if (graph != NULL)
    {
        visualize_kosaraju(graph);
        free_graph(graph);
    }
}

void scc_demo(void)
{
    Graph* graph = get_scc_demo_graph();
    if (graph == NULL)
    {
        return;
    }

    int choice;
    while (1)
    {
        int choice_status = safe_input_int(&choice,
                                           "\nChoose SCC Algorithm to Visualize:\n"
                                           "1. Tarjan's Algorithm\n"
                                           "2. Kosaraju's Algorithm\n"
                                           "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                           1, 2);
        if (choice_status == INPUT_EXIT_SIGNAL)
        {
            break;
        }
        if (choice_status == 0)
        {
            continue;
        }
        if (choice == 1)
        {
            visualize_tarjan(graph);
        }
        else if (choice == 2)
        {
            visualize_kosaraju(graph);
        }
        break;
    }

    free_graph(graph);
}
