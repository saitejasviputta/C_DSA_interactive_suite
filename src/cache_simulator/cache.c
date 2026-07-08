#include "cache.h"
#include <stdio.h>

void cache_init(Cache* cache, int capacity)
{
    if (capacity > CACHE_MAX_CAPACITY)
    {
        capacity = CACHE_MAX_CAPACITY;
    }
    if (capacity < 1)
    {
        capacity = 1;
    }
    cache->capacity = capacity;
    cache->size = 0;
    cache->fifo_index = 0;
    cache->hits = 0;
    cache->misses = 0;
    for (int i = 0; i < CACHE_MAX_CAPACITY; i++)
    {
        cache->blocks[i].page_id = -1;
        cache->blocks[i].is_valid = false;
        cache->blocks[i].is_dirty = false;
        cache->blocks[i].reference_bit = 0;
        cache->blocks[i].frequency = 0;
        cache->blocks[i].last_access_time = 0;
    }
}

bool cache_access_fifo(Cache* cache, int page_id, bool is_write)
{
    // Search for page in the cache (Hit check)
    for (int i = 0; i < cache->capacity; i++)
    {
        if (cache->blocks[i].is_valid && cache->blocks[i].page_id == page_id)
        {
            cache->hits++;
            if (is_write)
            {
                cache->blocks[i].is_dirty = true;
            }
            return true; // Hit
        }
    }

    // Cache Miss
    cache->misses++;

    // If cache is not full, insert in the first invalid slot
    if (cache->size < cache->capacity)
    {
        for (int i = 0; i < cache->capacity; i++)
        {
            if (!cache->blocks[i].is_valid)
            {
                cache->blocks[i].page_id = page_id;
                cache->blocks[i].is_valid = true;
                cache->blocks[i].is_dirty = is_write;
                cache->size++;
                return false; // Miss (inserted without eviction)
            }
        }
    }

    // Cache is full: Evict using FIFO policy
    int evict_idx = cache->fifo_index;

    // Perform eviction
    cache->blocks[evict_idx].page_id = page_id;
    cache->blocks[evict_idx].is_valid = true;
    cache->blocks[evict_idx].is_dirty = is_write;

    // Update FIFO pointer (circular queue)
    cache->fifo_index = (cache->fifo_index + 1) % cache->capacity;

    return false; // Miss (with eviction)
}

void cache_print_status(const Cache* cache)
{
    printf("Cache state: [ ");
    for (int i = 0; i < cache->capacity; i++)
    {
        if (cache->blocks[i].is_valid)
        {
            printf("%d ", cache->blocks[i].page_id);
        }
        else
        {
            printf("- ");
        }
    }
    printf("] (Hits: %d, Misses: %d)\n", cache->hits, cache->misses);
}
