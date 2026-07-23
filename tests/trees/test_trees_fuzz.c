#include "fuzzer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    unsigned int seed = (unsigned int)time(NULL);
    FuzzerState fuzzer;

    printf("Starting Tree Fuzzing with seed: %u\n", seed);

    // BST
    fuzzer_init(&fuzzer, seed, 5000);
    run_bst_fuzz(&fuzzer, 1000);
    fuzzer_free(&fuzzer);

    // AVL
    fuzzer_init(&fuzzer, seed, 5000);
    run_avl_fuzz(&fuzzer, 1000);
    fuzzer_free(&fuzzer);

    // Trie
    fuzzer_init(&fuzzer, seed, 5000);
    run_trie_fuzz(&fuzzer, 1000);
    fuzzer_free(&fuzzer);

    printf("Tree Fuzzing completed successfully!\n");
    return 0;
}
