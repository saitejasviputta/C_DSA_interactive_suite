#include "advanced_heaps.h"
#include "backtracking.h"
#include "cache.h"
#include "fuzzer.h"
#include "graph_traversals.h"
#include "hash.h"
#include "trees.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void run_bst_fuzz(FuzzerState* fuzzer, int ops)
{
    bstNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "BST insert %d", val);
            bst_insert(&root, val);
        }
        else if (op == 1)
        {
            fuzzer_log_op(fuzzer, "BST delete %d", val);
            root = bst_delete(root, val);
        }
        else
        {
            fuzzer_log_op(fuzzer, "BST height check");
            tree_height(root);
        }
    }
    destroy_bst(root);
}

void run_avl_fuzz(FuzzerState* fuzzer, int ops)
{
    avlNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "AVL insert %d", val);
            avl_insert(&root, val);
        }
        else if (op == 1)
        {
            fuzzer_log_op(fuzzer, "AVL delete %d", val);
            avl_delete(&root, val);
        }
        else
        {
            fuzzer_log_op(fuzzer, "AVL balance check");
            avl_balance_factor(root);
        }
    }
    destroy_avl(root);
}

void run_trie_fuzz(FuzzerState* fuzzer, int ops)
{
    TrieNode* root = trie_create_node();
    assert(root != NULL);

    char word[12];
    for (int i = 0; i < ops; i++)
    {
        int len = fuzzer_rand_int(fuzzer, 1, 10);
        for (int j = 0; j < len; j++)
        {
            word[j] = 'a' + fuzzer_rand_int(fuzzer, 0, 25);
        }
        word[len] = '\0';

        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "Trie insert %s", word);
            trie_insert(root, word);
        }
        else if (op == 1)
        {
            fuzzer_log_op(fuzzer, "Trie search %s", word);
            trie_search(root, word);
        }
        else
        {
            fuzzer_log_op(fuzzer, "Trie delete %s", word);
            trie_delete(root, word);
        }
    }
    trie_free(root);
}

void run_binomial_fuzz(FuzzerState* fuzzer, int ops)
{
    BinomialNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "Binomial insert %d:%d", key, val);
            root = binomial_heap_insert(root, key, val);
        }
        else if (op == 1 && root != NULL)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "Binomial extract min");
            root = binomial_heap_extract_min(root, &mk, &mv);
        }
        else
        {
            fuzzer_log_op(fuzzer, "Binomial find %d", key);
            binomial_heap_find_node(root, key);
        }
    }
    destroy_binomial_heap(root);
}

void run_fibonacci_fuzz(FuzzerState* fuzzer, int ops)
{
    FibonacciNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "Fibonacci insert %d:%d", key, val);
            root = fib_heap_insert(root, key, val);
        }
        else if (op == 1 && root != NULL)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "Fibonacci extract min");
            root = fib_heap_extract_min(root, &mk, &mv);
        }
        else if (op == 2 && root != NULL)
        {
            fuzzer_log_op(fuzzer, "Fibonacci delete %d", key);
            FibonacciNode* target = fib_heap_find_node(root, key);
            if (target != NULL)
            {
                root = fib_heap_delete(root, target);
            }
        }
    }
    destroy_fibonacci_heap(root);
}

void run_minmax_fuzz(FuzzerState* fuzzer, int ops)
{
    MinMaxHeap* heap = create_min_max_heap(ops + 10);
    assert(heap != NULL);
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "MinMax insert %d:%d", key, val);
            min_max_heap_insert(heap, key, val);
        }
        else if (op == 1)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "MinMax extract min");
            min_max_heap_extract_min(heap, &mk, &mv);
        }
        else
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "MinMax extract max");
            min_max_heap_extract_max(heap, &mk, &mv);
        }
    }
    destroy_min_max_heap(heap);
}

void run_leftist_fuzz(FuzzerState* fuzzer, int ops)
{
    LeftistNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 1);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "Leftist insert %d:%d", key, val);
            root = leftist_heap_insert(root, key, val);
        }
        else if (op == 1 && root != NULL)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "Leftist extract min");
            root = leftist_heap_extract_min(root, &mk, &mv);
        }
    }
    destroy_leftist_heap(root);
}

void run_skew_fuzz(FuzzerState* fuzzer, int ops)
{
    SkewNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 1);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "Skew insert %d:%d", key, val);
            root = skew_heap_insert(root, key, val);
        }
        else if (op == 1 && root != NULL)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "Skew extract min");
            root = skew_heap_extract_min(root, &mk, &mv);
        }
    }
    destroy_skew_heap(root);
}

void run_dary_fuzz(FuzzerState* fuzzer, int ops)
{
    DAryHeap* heap = create_dary_heap(ops + 10, 3);
    assert(heap != NULL);
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "d-Ary insert %d:%d", key, val);
            dary_heap_insert(heap, key, val);
        }
        else if (op == 1)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "d-Ary extract min");
            dary_heap_extract_min(heap, &mk, &mv);
        }
        else
        {
            fuzzer_log_op(fuzzer, "d-Ary find index %d", key);
            dary_heap_find_index(heap, key);
        }
    }
    destroy_dary_heap(heap);
}

void run_treap_fuzz(FuzzerState* fuzzer, int ops)
{
    TreapNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int pri = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "Treap insert %d:%d:%d", key, val, pri);
            root = treap_insert(root, key, val, pri);
        }
        else if (op == 1)
        {
            fuzzer_log_op(fuzzer, "Treap delete %d", key);
            root = treap_delete(root, key);
        }
        else
        {
            fuzzer_log_op(fuzzer, "Treap find %d", key);
            treap_find_node(root, key);
        }
    }
    destroy_treap(root);
}

void run_unweighted_graph_fuzz(FuzzerState* fuzzer, int num_nodes, int num_edges)
{
    Graph* graph = create_graph(num_nodes);
    assert(graph != NULL);

    for (int i = 0; i < num_edges; i++)
    {
        int u = fuzzer_rand_int(fuzzer, 0, num_nodes - 1);
        int v = fuzzer_rand_int(fuzzer, 0, num_nodes - 1);
        int dir = fuzzer_rand_int(fuzzer, 0, 1);
        if (dir == 0)
        {
            fuzzer_log_op(fuzzer, "Add undirected edge %d-%d", u, v);
            add_edge_undirected(graph, u, v);
        }
        else
        {
            fuzzer_log_op(fuzzer, "Add directed edge %d->%d", u, v);
            add_edge_directed_unweighted(graph, u, v);
        }
    }

    int start = fuzzer_rand_int(fuzzer, 0, num_nodes - 1);
    fuzzer_log_op(fuzzer, "BFS traversal from %d", start);
    bfs(graph, start);

    fuzzer_log_op(fuzzer, "DFS traversal from %d", start);
    dfs(graph, start);

    fuzzer_log_op(fuzzer, "Topological sort Kahn");
    topological_sort_kahn(graph);

    free_graph(graph);
}

void run_weighted_graph_fuzz(FuzzerState* fuzzer, int num_nodes, int num_edges)
{
    weightedGraph* graph = create_weightedGraph(num_nodes);
    assert(graph != NULL);

    for (int i = 0; i < num_edges; i++)
    {
        int u = fuzzer_rand_int(fuzzer, 0, num_nodes - 1);
        int v = fuzzer_rand_int(fuzzer, 0, num_nodes - 1);
        int w = fuzzer_rand_int(fuzzer, 1, 100);
        fuzzer_log_op(fuzzer, "Add weighted edge %d->%d weight %d", u, v, w);
        add_edge_directed(graph, u, v, w);
    }

    int start = fuzzer_rand_int(fuzzer, 0, num_nodes - 1);
    fuzzer_log_op(fuzzer, "Dijkstra from %d", start);
    dijkstra(graph, start);

    fuzzer_log_op(fuzzer, "Bellman-Ford from %d", start);
    bellman_ford(graph, start);

    fuzzer_log_op(fuzzer, "Kruskal MST");
    kruskal_mst(graph);

    fuzzer_log_op(fuzzer, "Prim MST from %d", start);
    prim_mst(graph, start);

    free_weightedGraph(graph);
}

void run_cache_fuzz(FuzzerState* fuzzer, int ops)
{
    Cache cache_fifo;
    Cache cache_lru;
    Cache cache_mru;
    Cache cache_lfu;
    Cache cache_clock;
    Cache cache_eclock;
    Cache cache_opt;

    int capacity = 5;
    cache_init(&cache_fifo, capacity);
    cache_init(&cache_lru, capacity);
    cache_init(&cache_mru, capacity);
    cache_init(&cache_lfu, capacity);
    cache_init(&cache_clock, capacity);
    cache_init(&cache_eclock, capacity);
    cache_init(&cache_opt, capacity);

    int ref_len = ops;
    int* ref_str = (int*)malloc(ref_len * sizeof(int));
    assert(ref_str != NULL);
    for (int i = 0; i < ref_len; i++)
    {
        ref_str[i] = fuzzer_rand_int(fuzzer, 1, 15);
    }

    for (int i = 0; i < ops; i++)
    {
        int page_id = ref_str[i];
        bool is_write = (fuzzer_rand_int(fuzzer, 0, 1) == 1);

        fuzzer_log_op(fuzzer, "Cache access page_id %d write %d", page_id, is_write);

        cache_access_fifo(&cache_fifo, page_id, is_write);
        cache_access_lru(&cache_lru, page_id, is_write);
        cache_access_mru(&cache_mru, page_id, is_write);
        cache_access_lfu(&cache_lfu, page_id, is_write);
        cache_access_clock(&cache_clock, page_id, is_write);
        cache_access_enhanced_clock(&cache_eclock, page_id, is_write);
        cache_access_opt(&cache_opt, page_id, ref_str, ref_len, i, is_write);
    }

    free(ref_str);
}

void run_hashing_fuzz(FuzzerState* fuzzer, int ops)
{
    for (int i = 0; i < ops; i++)
    {
        int val = fuzzer_rand_int(fuzzer, -5000, 5000);
        int size = fuzzer_rand_int(fuzzer, 1, 500);
        fuzzer_log_op(fuzzer, "Hash value %d size %d", val, size);
        int h = hash_function(val, size);
        assert(h >= 0 && h < size);
    }
}
void run_sudoku_fuzz(FuzzerState* fuzzer, int runs)
{
    int grid[6][6];
    for (int r = 0; r < runs; r++)
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (fuzzer_rand_int(fuzzer, 0, 4) == 0)
                {
                    grid[i][j] = fuzzer_rand_int(fuzzer, 1, 6);
                }
                else
                {
                    grid[i][j] = 0;
                }
            }
        }
        fuzzer_log_op(fuzzer, "Sudoku run %d", r);
        run_sudoku_test(grid);
    }
}

void run_maze_fuzz(FuzzerState* fuzzer, int runs)
{
    int maze[6][6];
    for (int r = 0; r < runs; r++)
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if ((i == 0 && j == 0) || (i == 5 && j == 5))
                {
                    maze[i][j] = 1;
                }
                else
                {
                    maze[i][j] = (fuzzer_rand_int(fuzzer, 0, 3) == 0) ? 0 : 1;
                }
            }
        }
        fuzzer_log_op(fuzzer, "Maze run %d", r);
        run_rat_in_maze_test(maze);
    }
}
