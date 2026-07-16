#include "advanced_graph_algorithms.h"
#include "clear_screen.h"
#include "config.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_flow_state(weightedGraph* graph, int** residual, int source, int sink,
                             int* parent, int curr_v, const char* phase)
{
    clear_screen();
    printf("\n=== Maximum Flow Visualizer (%s) ===\n\n", phase);
    printf("Source Node: %d | Sink Node: %d\n\n", source, sink);

    int V = graph->V;

    // Print capacities and current flow
    printf("Edge flows and capacities:\n");
    for (int u = 0; u < V; u++)
    {
        Edge* temp = graph->array[u];
        while (temp != NULL)
        {
            int v = temp->destination;
            int capacity = temp->weight;
            int current_flow = capacity - residual[u][v];

            // Highlight active nodes/edges in the path
            bool in_path = false;
            if (parent != NULL)
            {
                for (int curr = sink; curr != source && parent[curr] != -1; curr = parent[curr])
                {
                    if (parent[curr] == u && curr == v)
                    {
                        in_path = true;
                        break;
                    }
                }
            }

            if (u == curr_v || v == curr_v)
            {
                printf("  \033[1;31m%d -> %d: Flow = %d / %d\033[0m\n", u, v, current_flow,
                       capacity);
            }
            else if (in_path)
            {
                printf("  \033[1;32m%d -> %d: Flow = %d / %d (Augmenting Edge)\033[0m\n", u, v,
                       current_flow, capacity);
            }
            else
            {
                printf("  %d -> %d: Flow = %d / %d\n", u, v, current_flow, capacity);
            }
            temp = temp->next;
        }
    }
    printf("\n");

    printf("Residual Capacity Matrix:\n    ");
    for (int i = 0; i < V; i++)
        printf(" %2d ", i);
    printf("\n    ");
    for (int i = 0; i < V; i++)
        printf("----");
    printf("\n");
    for (int i = 0; i < V; i++)
    {
        printf(" %2d |", i);
        for (int j = 0; j < V; j++)
        {
            if (residual[i][j] > 0)
            {
                printf("\033[1;33m %2d \033[0m", residual[i][j]);
            }
            else
            {
                printf("  0 ");
            }
        }
        printf("\n");
    }
    printf("\n");

    if (parent != NULL && parent[sink] != -1)
    {
        printf("Current Augmenting Path: ");
        int path[100];
        int path_len = 0;
        for (int curr = sink; curr != -1; curr = parent[curr])
        {
            path[path_len++] = curr;
        }
        for (int i = path_len - 1; i >= 0; i--)
        {
            printf("%d", path[i]);
            if (i > 0)
                printf(" -> ");
        }
        printf("\n");
    }

    fflush(stdout);
    dynamic_sleep();
}

static bool dfs_augmenting_path_vis(weightedGraph* graph, int** residual, int u, int sink,
                                    bool* visited, int* parent, const char* phase)
{
    if (u == sink)
        return true;

    visited[u] = true;
    print_flow_state(graph, residual, parent[sink] == -1 ? u : parent[sink], sink, parent, u,
                     phase);

    int V = graph->V;
    for (int v = 0; v < V; v++)
    {
        if (!visited[v] && residual[u][v] > 0)
        {
            parent[v] = u;
            if (dfs_augmenting_path_vis(graph, residual, v, sink, visited, parent, phase))
            {
                return true;
            }
        }
    }
    return false;
}

void visualize_ford_fulkerson(weightedGraph* graph, int source, int sink)
{
    int V = graph->V;
    int** residual = malloc(sizeof(int*) * V);
    if (residual == NULL)
        return;
    for (int i = 0; i < V; i++)
    {
        residual[i] = calloc(V, sizeof(int));
        if (residual[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(residual[j]);
            free(residual);
            return;
        }
    }

    for (int u = 0; u < V; u++)
    {
        Edge* temp = graph->array[u];
        while (temp != NULL)
        {
            residual[u][temp->destination] = temp->weight;
            temp = temp->next;
        }
    }

    int* parent = malloc(sizeof(int) * V);
    bool* visited = malloc(sizeof(bool) * V);
    if (parent == NULL || visited == NULL)
    {
        free(parent);
        free(visited);
        for (int i = 0; i < V; i++)
            free(residual[i]);
        free(residual);
        return;
    }
    int max_flow = 0;

    while (1)
    {
        memset(visited, 0, sizeof(bool) * V);
        for (int i = 0; i < V; i++)
            parent[i] = -1;

        print_flow_state(graph, residual, source, sink, NULL, -1, "Ford-Fulkerson searching path");
        if (!dfs_augmenting_path_vis(graph, residual, source, sink, visited, parent,
                                     "Ford-Fulkerson Path Found"))
        {
            break;
        }

        int path_flow = INT_MAX;
        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            if (residual[u][v] < path_flow)
            {
                path_flow = residual[u][v];
            }
        }

        print_flow_state(graph, residual, source, sink, parent, -1,
                         "Ford-Fulkerson (Bottleneck calculated)");

        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            residual[u][v] -= path_flow;
            residual[v][u] += path_flow;
        }

        max_flow += path_flow;
        print_flow_state(graph, residual, source, sink, parent, -1,
                         "Ford-Fulkerson (Flow augmented)");
    }

    clear_screen();
    printf("\n=== Ford-Fulkerson Max Flow Final Result ===\n\n");
    printf("Source: %d | Sink: %d\n", source, sink);
    printf("Max Flow: %d\n\n", max_flow);
    printf("Press Enter to continue...");
    fflush(stdout);
    getchar();

    for (int i = 0; i < V; i++)
        free(residual[i]);
    free(residual);
    free(parent);
    free(visited);
}

static bool bfs_augmenting_path_vis(weightedGraph* graph, int** residual, int source, int sink,
                                    int* parent, const char* phase)
{
    int V = graph->V;
    bool* visited = calloc(V, sizeof(bool));
    int* queue = malloc(sizeof(int) * V);
    if (visited == NULL || queue == NULL)
    {
        free(visited);
        free(queue);
        return false;
    }
    int head = 0, tail = 0;

    queue[tail++] = source;
    visited[source] = true;

    while (head < tail)
    {
        int u = queue[head++];
        print_flow_state(graph, residual, source, sink, parent, u, phase);

        if (u == sink)
        {
            free(visited);
            free(queue);
            return true;
        }

        for (int v = 0; v < V; v++)
        {
            if (!visited[v] && residual[u][v] > 0)
            {
                parent[v] = u;
                visited[v] = true;
                queue[tail++] = v;
            }
        }
    }

    free(visited);
    free(queue);
    return false;
}

void visualize_edmonds_karp(weightedGraph* graph, int source, int sink)
{
    int V = graph->V;
    int** residual = malloc(sizeof(int*) * V);
    if (residual == NULL)
        return;
    for (int i = 0; i < V; i++)
    {
        residual[i] = calloc(V, sizeof(int));
        if (residual[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(residual[j]);
            free(residual);
            return;
        }
    }

    for (int u = 0; u < V; u++)
    {
        Edge* temp = graph->array[u];
        while (temp != NULL)
        {
            residual[u][temp->destination] = temp->weight;
            temp = temp->next;
        }
    }

    int* parent = malloc(sizeof(int) * V);
    if (parent == NULL)
    {
        for (int i = 0; i < V; i++)
            free(residual[i]);
        free(residual);
        return;
    }
    int max_flow = 0;

    while (1)
    {
        for (int i = 0; i < V; i++)
            parent[i] = -1;

        print_flow_state(graph, residual, source, sink, NULL, -1, "Edmonds-Karp searching path");
        if (!bfs_augmenting_path_vis(graph, residual, source, sink, parent,
                                     "Edmonds-Karp Path Found"))
        {
            break;
        }

        int path_flow = INT_MAX;
        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            if (residual[u][v] < path_flow)
            {
                path_flow = residual[u][v];
            }
        }

        print_flow_state(graph, residual, source, sink, parent, -1,
                         "Edmonds-Karp (Bottleneck calculated)");

        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            residual[u][v] -= path_flow;
            residual[v][u] += path_flow;
        }

        max_flow += path_flow;
        print_flow_state(graph, residual, source, sink, parent, -1,
                         "Edmonds-Karp (Flow augmented)");
    }

    clear_screen();
    printf("\n=== Edmonds-Karp Max Flow Final Result ===\n\n");
    printf("Source: %d | Sink: %d\n", source, sink);
    printf("Max Flow: %d\n\n", max_flow);
    printf("Press Enter to continue...");
    fflush(stdout);
    getchar();

    for (int i = 0; i < V; i++)
        free(residual[i]);
    free(residual);
    free(parent);
}

static void print_dinic_state(weightedGraph* graph, int** residual, int source, int sink,
                              int* level, int* parent, int curr_v, const char* phase)
{
    clear_screen();
    printf("\n=== Dinic's Algorithm Visualizer (%s) ===\n\n", phase);
    printf("Source Node: %d | Sink Node: %d\n\n", source, sink);

    int V = graph->V;

    printf("Vertex levels: ");
    for (int i = 0; i < V; i++)
    {
        if (level[i] != -1)
        {
            printf("[%d]: Lvl %d | ", i, level[i]);
        }
        else
        {
            printf("[%d]: Unreachable | ", i);
        }
    }
    printf("\n\n");

    printf("Edge flows and capacities:\n");
    for (int u = 0; u < V; u++)
    {
        Edge* temp = graph->array[u];
        while (temp != NULL)
        {
            int v = temp->destination;
            int capacity = temp->weight;
            int current_flow = capacity - residual[u][v];

            bool is_level_edge = (level[u] != -1 && level[v] != -1 && level[v] == level[u] + 1);
            if (u == curr_v || v == curr_v)
            {
                printf("  \033[1;31m%d -> %d: Flow = %d / %d\033[0m\n", u, v, current_flow,
                       capacity);
            }
            else if (is_level_edge && residual[u][v] > 0)
            {
                printf("  \033[1;32m%d -> %d: Flow = %d / %d (Level Graph Edge)\033[0m\n", u, v,
                       current_flow, capacity);
            }
            else
            {
                printf("  %d -> %d: Flow = %d / %d\n", u, v, current_flow, capacity);
            }
            temp = temp->next;
        }
    }
    printf("\n");

    printf("Residual Capacity Matrix:\n    ");
    for (int i = 0; i < V; i++)
        printf(" %2d ", i);
    printf("\n    ");
    for (int i = 0; i < V; i++)
        printf("----");
    printf("\n");
    for (int i = 0; i < V; i++)
    {
        printf(" %2d |", i);
        for (int j = 0; j < V; j++)
        {
            if (residual[i][j] > 0)
            {
                printf("\033[1;33m %2d \033[0m", residual[i][j]);
            }
            else
            {
                printf("  0 ");
            }
        }
        printf("\n");
    }
    printf("\n");

    if (parent != NULL && parent[sink] != -1)
    {
        printf("Current DFS Path: ");
        int path[100];
        int path_len = 0;
        for (int curr = sink; curr != -1; curr = parent[curr])
        {
            path[path_len++] = curr;
        }
        for (int i = path_len - 1; i >= 0; i--)
        {
            printf("%d", path[i]);
            if (i > 0)
                printf(" -> ");
        }
        printf("\n");
    }

    fflush(stdout);
    dynamic_sleep();
}

static bool dinic_bfs_vis(weightedGraph* graph, int** residual, int source, int sink, int* level)
{
    int V = graph->V;
    for (int i = 0; i < V; i++)
        level[i] = -1;
    level[source] = 0;

    int* queue = malloc(sizeof(int) * V);
    if (queue == NULL)
        return false;
    int head = 0, tail = 0;
    queue[tail++] = source;

    while (head < tail)
    {
        int u = queue[head++];
        print_dinic_state(graph, residual, source, sink, level, NULL, u,
                          "Dinic BFS (Building Level Graph)");
        for (int v = 0; v < V; v++)
        {
            if (level[v] < 0 && residual[u][v] > 0)
            {
                level[v] = level[u] + 1;
                queue[tail++] = v;
            }
        }
    }

    bool reachable = (level[sink] >= 0);
    free(queue);
    return reachable;
}

static int dinic_dfs_vis(weightedGraph* graph, int** residual, int u, int sink, int flow,
                         int* level, int* start, int source, int* parent)
{
    if (u == sink)
        return flow;

    int V = graph->V;
    print_dinic_state(graph, residual, source, sink, level, parent, u,
                      "Dinic DFS (Finding blocking path)");

    for (; start[u] < V; start[u]++)
    {
        int v = start[u];
        if (level[v] == level[u] + 1 && residual[u][v] > 0)
        {
            parent[v] = u;
            int curr_flow = flow < residual[u][v] ? flow : residual[u][v];
            int temp_flow =
                dinic_dfs_vis(graph, residual, v, sink, curr_flow, level, start, source, parent);
            if (temp_flow > 0)
            {
                residual[u][v] -= temp_flow;
                residual[v][u] += temp_flow;
                return temp_flow;
            }
        }
    }
    return 0;
}

void visualize_dinic(weightedGraph* graph, int source, int sink)
{
    int V = graph->V;
    int** residual = malloc(sizeof(int*) * V);
    if (residual == NULL)
        return;
    for (int i = 0; i < V; i++)
    {
        residual[i] = calloc(V, sizeof(int));
        if (residual[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(residual[j]);
            free(residual);
            return;
        }
    }

    for (int u = 0; u < V; u++)
    {
        Edge* temp = graph->array[u];
        while (temp != NULL)
        {
            residual[u][temp->destination] = temp->weight;
            temp = temp->next;
        }
    }

    int* level = malloc(sizeof(int) * V);
    int* start = malloc(sizeof(int) * V);
    int* parent = malloc(sizeof(int) * V);
    if (level == NULL || start == NULL || parent == NULL)
    {
        free(level);
        free(start);
        free(parent);
        for (int i = 0; i < V; i++)
            free(residual[i]);
        free(residual);
        return;
    }
    int max_flow = 0;

    while (dinic_bfs_vis(graph, residual, source, sink, level))
    {
        memset(start, 0, sizeof(int) * V);
        while (1)
        {
            for (int i = 0; i < V; i++)
                parent[i] = -1;
            int flow =
                dinic_dfs_vis(graph, residual, source, sink, INT_MAX, level, start, source, parent);
            if (flow == 0)
                break;
            max_flow += flow;
            print_dinic_state(graph, residual, source, sink, level, parent, -1,
                              "Dinic (Augmented blocking flow)");
        }
    }

    clear_screen();
    printf("\n=== Dinic's Max Flow Final Result ===\n\n");
    printf("Source: %d | Sink: %d\n", source, sink);
    printf("Max Flow: %d\n\n", max_flow);
    printf("Press Enter to continue...");
    fflush(stdout);
    getchar();

    for (int i = 0; i < V; i++)
        free(residual[i]);
    free(residual);
    free(level);
    free(start);
    free(parent);
}
