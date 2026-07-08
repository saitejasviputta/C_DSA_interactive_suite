#include "cache.h"
#include <assert.h>
#include <stdio.h>

void test_fifo_basic()
{
    Cache cache;
    cache_init(&cache, 3);

    // Initial state
    assert(cache.capacity == 3);
    assert(cache.size == 0);
    assert(cache.hits == 0);
    assert(cache.misses == 0);

    // Access 1: Miss
    assert(!cache_access_fifo(&cache, 1, false));
    assert(cache.size == 1);
    assert(cache.misses == 1);

    // Access 2: Miss
    assert(!cache_access_fifo(&cache, 2, false));
    assert(cache.size == 2);
    assert(cache.misses == 2);

    // Access 3: Miss
    assert(!cache_access_fifo(&cache, 3, false));
    assert(cache.size == 3);
    assert(cache.misses == 3);

    // Access 1: Hit
    assert(cache_access_fifo(&cache, 1, false));
    assert(cache.size == 3);
    assert(cache.hits == 1);

    // Access 4: Miss (FIFO evicts 1)
    assert(!cache_access_fifo(&cache, 4, false));
    assert(cache.misses == 4);

    // 1 should no longer be in the cache
    bool found_1 = false;
    for (int i = 0; i < 3; i++)
    {
        if (cache.blocks[i].page_id == 1)
            found_1 = true;
    }
    assert(!found_1);

    // Access 1: Miss (FIFO evicts 2)
    assert(!cache_access_fifo(&cache, 1, false));
    assert(cache.misses == 5);

    // 2 should no longer be in cache
    bool found_2 = false;
    for (int i = 0; i < 3; i++)
    {
        if (cache.blocks[i].page_id == 2)
            found_2 = true;
    }
    assert(!found_2);

    printf("FIFO basic cache tests passed\n");
}

int main()
{
    test_fifo_basic();
    printf("All cache simulator tests passed\n");
    return 0;
}
