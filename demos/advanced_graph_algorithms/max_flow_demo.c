#include "advanced_graph_algorithms.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static weightedGraph* get_max_flow_demo_graph(int* out_graph_capacity)
{
    int edges;
    int graph_capacity = 0;
    int input_method;
    weightedGraph* graph = NULL;

    while (1)
    {
        int method_status = safe_input_int(&input_method,
                                           "\nenter 1 to build the graph manually, 2 to load it "
                                           "from a CSV file, enter '-1' to exit : ",
                                           1, 2);

        if (method_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Max Flow visualization demo.....\n");
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
                printf("\nExiting Max Flow visualization demo.....\n");
                return NULL;
            }

            if (len == 0)
            {
                continue;
            }

            graph = load_weightedGraph_from_csv(path);

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
                               "(between 2 and 10), enter '-1' to exit : ",
                               2, 10);

            if (graph_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Max Flow visualization demo.....\n");
                return NULL;
            }

            if (graph_capacity_status == 0)
            {
                continue;
            }

            graph = create_weightedGraph(graph_capacity);
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
                printf("\nExiting Max Flow visualization demo\n");
                free_weightedGraph(graph);
                return NULL;
            }

            if (edges_capacity_status == 0)
            {
                continue;
            }

            break;
        }

        printf("\nenter directed edges (src dest weight) (vertices from 0 to %d, enter '-1' to "
               "exit):\n",
               graph_capacity - 1);

        for (int i = 0; i < edges; i++)
        {
            int src_status;
            int dest_status;
            int wt_status;
            int src;
            int dest;
            int wt;

        retry:
            src_status = safe_input_int(&src, "src: ", 0, graph_capacity - 1);

            if (src_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Max Flow visualization demo\n");
                free_weightedGraph(graph);
                return NULL;
            }
            if (src_status == 0)
            {
                goto retry;
            }

            dest_status = safe_input_int(&dest, "dest: ", 0, graph_capacity - 1);

            if (dest_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Max Flow visualization demo\n");
                free_weightedGraph(graph);
                return NULL;
            }
            if (dest_status == 0)
            {
                goto retry;
            }

            wt_status = safe_input_int(&wt, "weight: ", 1, INT_MAX);
            if (wt_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Max Flow visualization demo\n");
                free_weightedGraph(graph);
                return NULL;
            }
            if (wt_status == 0)
            {
                goto retry;
            }

            add_edge_directed(graph, src, dest, wt);
        }
    }

    *out_graph_capacity = graph_capacity;
    return graph;
}

static bool get_source_sink(int graph_capacity, int* out_source, int* out_sink)
{
    int source, sink;
    while (1)
    {
        int src_status = safe_input_int(&source, "Enter source node: ", 0, graph_capacity - 1);
        if (src_status == INPUT_EXIT_SIGNAL)
        {
            return false;
        }
        if (src_status == 0)
            continue;

        int sink_status = safe_input_int(&sink, "Enter sink node: ", 0, graph_capacity - 1);
        if (sink_status == INPUT_EXIT_SIGNAL)
        {
            return false;
        }
        if (sink_status == 0)
            continue;

        if (source == sink)
        {
            printf("Source and Sink cannot be the same!\n");
            continue;
        }
        break;
    }
    *out_source = source;
    *out_sink = sink;
    return true;
}

void ford_fulkerson_demo(void)
{
    int graph_capacity;
    weightedGraph* graph = get_max_flow_demo_graph(&graph_capacity);
    if (graph == NULL)
        return;

    int source, sink;
    if (get_source_sink(graph_capacity, &source, &sink))
    {
        visualize_ford_fulkerson(graph, source, sink);
    }
    free_weightedGraph(graph);
}

void edmonds_karp_demo(void)
{
    int graph_capacity;
    weightedGraph* graph = get_max_flow_demo_graph(&graph_capacity);
    if (graph == NULL)
        return;

    int source, sink;
    if (get_source_sink(graph_capacity, &source, &sink))
    {
        visualize_edmonds_karp(graph, source, sink);
    }
    free_weightedGraph(graph);
}

void dinic_demo(void)
{
    int graph_capacity;
    weightedGraph* graph = get_max_flow_demo_graph(&graph_capacity);
    if (graph == NULL)
        return;

    int source, sink;
    if (get_source_sink(graph_capacity, &source, &sink))
    {
        visualize_dinic(graph, source, sink);
    }
    free_weightedGraph(graph);
}

void max_flow_demo(void)
{
    int graph_capacity;
    weightedGraph* graph = get_max_flow_demo_graph(&graph_capacity);
    if (graph == NULL)
        return;

    int source, sink;
    if (!get_source_sink(graph_capacity, &source, &sink))
    {
        free_weightedGraph(graph);
        return;
    }

    int choice;
    while (1)
    {
        int choice_status = safe_input_int(&choice,
                                           "\nChoose Maximum Flow Algorithm to Visualize:\n"
                                           "1. Ford-Fulkerson Algorithm (DFS-based)\n"
                                           "2. Edmonds-Karp Algorithm (BFS-based)\n"
                                           "3. Dinic's Algorithm (Level Graph + Blocking Flow)\n"
                                           "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                           1, 3);
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
            visualize_ford_fulkerson(graph, source, sink);
        }
        else if (choice == 2)
        {
            visualize_edmonds_karp(graph, source, sink);
        }
        else if (choice == 3)
        {
            visualize_dinic(graph, source, sink);
        }
        break;
    }

    free_weightedGraph(graph);
}
