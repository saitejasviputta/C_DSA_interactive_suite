#include "advanced_graph_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

static Graph* get_bipartite_matching_graph(void)
{
    int vertices, edges;
    Graph* graph = NULL;

    int input_status = safe_input_int(&vertices, "Enter number of vertices (2-20): ", 2, 20);
    if (input_status == INPUT_EXIT_SIGNAL)
        return NULL;

    graph = create_graph(vertices);
    if (graph == NULL)
    {
        printf("\nError: Memory allocation failed for graph.\n");
        return NULL;
    }
    int edge_status = safe_input_int(&edges, "Enter number of edges (1-100): ", 1, 100);
    if (edge_status == INPUT_EXIT_SIGNAL)
    {
        free_graph(graph);
        return NULL;
    }

    printf("Enter undirected edges (u v) with vertices from 0 to %d:\n", vertices - 1);
    for (int i = 0; i < edges; i++)
    {
        int u, v;
        if (safe_input_int(&u, "u: ", 0, vertices - 1) == INPUT_EXIT_SIGNAL)
        {
            free_graph(graph);
            return NULL;
        }
        if (safe_input_int(&v, "v: ", 0, vertices - 1) == INPUT_EXIT_SIGNAL)
        {
            free_graph(graph);
            return NULL;
        }
        add_edge_undirected(graph, u, v);
    }
    return graph;
}

void dinic_bipartite_matching_demo(void)
{
    Graph* graph = get_bipartite_matching_graph();
    if (graph == NULL)
        return;

    printf("\nMaximum Bipartite Matching Demo (Dinic's Flow Network)\n");

    int match_count_dinic = 0;
    int* match_pairs_dinic = NULL;
    int max_dinic = max_bipartite_matching(graph, &match_pairs_dinic, &match_count_dinic);

    printf("\n=== Bipartite Matching Results (Dinic's) ===\n");
    printf("Max Matching: %d\n", max_dinic);
    if (match_count_dinic > 0)
    {
        printf("Matching Pairs: ");
        for (int i = 0; i < match_count_dinic; i++)
        {
            printf("(%d - %d) ", match_pairs_dinic[2 * i], match_pairs_dinic[2 * i + 1]);
        }
        printf("\n");
        free(match_pairs_dinic);
    }

    free_graph(graph);
    printf("\nPress Enter to continue...");
    getchar();
}

void hopcroft_karp_demo(void)
{
    Graph* graph = get_bipartite_matching_graph();
    if (graph == NULL)
        return;

    printf("\nMaximum Bipartite Matching Demo (Hopcroft-Karp)\n");

    int match_count_hk = 0;
    int* match_pairs_hk = NULL;
    int max_hk = hopcroft_karp(graph, &match_pairs_hk, &match_count_hk);

    printf("\n=== Bipartite Matching Results (Hopcroft-Karp) ===\n");
    printf("Max Matching: %d\n", max_hk);
    if (match_count_hk > 0)
    {
        printf("Matching Pairs: ");
        for (int i = 0; i < match_count_hk; i++)
        {
            printf("(%d - %d) ", match_pairs_hk[2 * i], match_pairs_hk[2 * i + 1]);
        }
        printf("\n");
        free(match_pairs_hk);
    }

    free_graph(graph);
    printf("\nPress Enter to continue...");
    getchar();
}

void bipartite_matching_demo(void)
{
    Graph* graph = get_bipartite_matching_graph();
    if (graph == NULL)
        return;

    printf("\nMaximum Bipartite Matching Demo\n");

    int match_count_dinic = 0;
    int* match_pairs_dinic = NULL;
    int max_dinic = max_bipartite_matching(graph, &match_pairs_dinic, &match_count_dinic);

    int match_count_hk = 0;
    int* match_pairs_hk = NULL;
    int max_hk = hopcroft_karp(graph, &match_pairs_hk, &match_count_hk);

    printf("\n=== Bipartite Matching Results ===\n");
    printf("Max Matching (Dinic's Flow Network): %d\n", max_dinic);
    if (match_count_dinic > 0)
    {
        printf("Matching Pairs (Dinic's): ");
        for (int i = 0; i < match_count_dinic; i++)
        {
            printf("(%d - %d) ", match_pairs_dinic[2 * i], match_pairs_dinic[2 * i + 1]);
        }
        printf("\n");
        free(match_pairs_dinic);
    }

    printf("Max Matching (Hopcroft-Karp): %d\n", max_hk);
    if (match_count_hk > 0)
    {
        printf("Matching Pairs (Hopcroft-Karp): ");
        for (int i = 0; i < match_count_hk; i++)
        {
            printf("(%d - %d) ", match_pairs_hk[2 * i], match_pairs_hk[2 * i + 1]);
        }
        printf("\n");
        free(match_pairs_hk);
    }

    free_graph(graph);
    printf("\nPress Enter to continue...");
    getchar();
}
