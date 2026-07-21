#include "cache.h"
#include "fuzzer.h"
#include "hash.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    // Generate random reference string for OPT cache simulator
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
