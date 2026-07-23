#include "fuzzer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    unsigned int seed = (unsigned int)time(NULL);
    FuzzerState fuzzer;
    printf("Starting Cache & Hashing Fuzzing with seed: %u\n", seed);

    fuzzer_init(&fuzzer, seed, 20000);
    run_cache_fuzz(&fuzzer, 1000);
    run_hashing_fuzz(&fuzzer, 1000);
    fuzzer_free(&fuzzer);

    printf("Cache & Hashing Fuzzing completed successfully!\n");
    return 0;
}
