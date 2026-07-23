#include "fuzzer.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Forward declarations of fuzz test runners
void run_bst_fuzz(FuzzerState* fuzzer, int ops);
void run_avl_fuzz(FuzzerState* fuzzer, int ops);
void run_trie_fuzz(FuzzerState* fuzzer, int ops);

void run_binomial_fuzz(FuzzerState* fuzzer, int ops);
void run_fibonacci_fuzz(FuzzerState* fuzzer, int ops);
void run_minmax_fuzz(FuzzerState* fuzzer, int ops);
void run_leftist_fuzz(FuzzerState* fuzzer, int ops);
void run_skew_fuzz(FuzzerState* fuzzer, int ops);
void run_dary_fuzz(FuzzerState* fuzzer, int ops);
void run_treap_fuzz(FuzzerState* fuzzer, int ops);

void run_unweighted_graph_fuzz(FuzzerState* fuzzer, int num_nodes, int num_edges);
void run_weighted_graph_fuzz(FuzzerState* fuzzer, int num_nodes, int num_edges);

void run_sudoku_fuzz(FuzzerState* fuzzer, int runs);
void run_maze_fuzz(FuzzerState* fuzzer, int runs);
void run_n_queens_test(int n);
void run_knights_tour_test(int n);

void run_cache_fuzz(FuzzerState* fuzzer, int ops);
void run_hashing_fuzz(FuzzerState* fuzzer, int ops);

void fuzzer_demo(void)
{
    while (1)
    {
        printf("\n=== STOCHASTIC FUZZ TESTING ENGINE ===\n");
        printf("1. Fuzz Tree Data Structures\n");
        printf("2. Fuzz Heap Data Structures\n");
        printf("3. Fuzz Graph Algorithms\n");
        printf("4. Fuzz Backtracking Solvers\n");
        printf("5. Fuzz Cache & Hashing Tables\n");
        printf("6. Fuzz All Modules\n");
        printf("-1. Back to Main Menu\n");

        int choice;
        int status = safe_input_int(&choice, "\nEnter your choice: ", 1, 6);
        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Fuzzer Demo...\n");
            return;
        }
        if (status == 0)
        {
            continue;
        }

        int custom_seed;
        status = safe_input_int(&custom_seed, "Enter seed (0 for random): ", 0, 100000);
        if (status == INPUT_EXIT_SIGNAL)
            return;

        unsigned int seed =
            (custom_seed == 0) ? (unsigned int)time(NULL) : (unsigned int)custom_seed;

        int ops;
        status = safe_input_int(&ops, "Enter number of operations (10 to 1000): ", 10, 1000);
        if (status == INPUT_EXIT_SIGNAL)
            return;

        // Temporarily set test mode to bypass dijkstra choice prompts
        setenv("DSA_TEST_MODE", "1", 1);

        FuzzerState fuzzer;
        fuzzer_init(&fuzzer, seed, ops * 10);

        printf("\nRunning fuzz tests with seed %u...\n", seed);

        if (choice == 1 || choice == 6)
        {
            printf("Fuzzing BST...\n");
            run_bst_fuzz(&fuzzer, ops);
            printf("Fuzzing AVL...\n");
            run_avl_fuzz(&fuzzer, ops);
            printf("Fuzzing Trie...\n");
            run_trie_fuzz(&fuzzer, ops);
        }
        if (choice == 2 || choice == 6)
        {
            printf("Fuzzing Binomial Heap...\n");
            run_binomial_fuzz(&fuzzer, ops);
            printf("Fuzzing Fibonacci Heap...\n");
            run_fibonacci_fuzz(&fuzzer, ops);
            printf("Fuzzing Min-Max Heap...\n");
            run_minmax_fuzz(&fuzzer, ops);
            printf("Fuzzing Leftist Heap...\n");
            run_leftist_fuzz(&fuzzer, ops);
            printf("Fuzzing Skew Heap...\n");
            run_skew_fuzz(&fuzzer, ops);
            printf("Fuzzing d-Ary Heap...\n");
            run_dary_fuzz(&fuzzer, ops);
            printf("Fuzzing Treap...\n");
            run_treap_fuzz(&fuzzer, ops);
        }
        if (choice == 3 || choice == 6)
        {
            printf("Fuzzing Unweighted Graphs...\n");
            run_unweighted_graph_fuzz(&fuzzer, 15, ops);
            printf("Fuzzing Weighted Graphs...\n");
            run_weighted_graph_fuzz(&fuzzer, 15, ops);
        }
        if (choice == 4 || choice == 6)
        {
            printf("Fuzzing N-Queens...\n");
            for (int i = 0; i < 5; i++)
            {
                run_n_queens_test(fuzzer_rand_int(&fuzzer, 4, 8));
            }
            printf("Fuzzing Knights Tour...\n");
            for (int i = 0; i < 5; i++)
            {
                run_knights_tour_test(fuzzer_rand_int(&fuzzer, 5, 6));
            }
            printf("Fuzzing Sudoku...\n");
            run_sudoku_fuzz(&fuzzer, 5);
            printf("Fuzzing Rat in Maze...\n");
            run_maze_fuzz(&fuzzer, 5);
        }
        if (choice == 5 || choice == 6)
        {
            printf("Fuzzing Caches...\n");
            run_cache_fuzz(&fuzzer, ops);
            printf("Fuzzing Hashing...\n");
            run_hashing_fuzz(&fuzzer, ops);
        }

        unsetenv("DSA_TEST_MODE");

        printf("\nFuzz run completed successfully with 0 crashes!\n");
        fuzzer_dump_log(&fuzzer);
        fuzzer_free(&fuzzer);
    }
}
