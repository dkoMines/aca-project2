//
// This file defines the function signatures necessary for creating the three
// cache systems and defines the prefetcher struct.
//

#include "prefetchers.h"

// Nul Prefetcher
// ============================================================================
uint32_t null_handle_mem_access(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                uint32_t address, bool is_miss)
{
    return 0; // No lines prefetched
}

void null_cleanup(struct prefetcher *prefetcher) {}

struct prefetcher *null_prefetcher_new()
{
    struct prefetcher *null_prefetcher = calloc(1, sizeof(struct prefetcher));
    null_prefetcher->handle_mem_access = &null_handle_mem_access;
    null_prefetcher->cleanup = &null_cleanup;
    return null_prefetcher;
}

// Sequential Prefetcher
// ============================================================================
// TODO feel free to create additional structs/enums as necessary

uint32_t sequential_handle_mem_access(struct prefetcher *prefetcher,
                                      struct cache_system *cache_system, uint32_t address,
                                      bool is_miss)
{
    // TODO: Return the number of lines that were prefetched.
    int count = 0;
    uint32_t pAmount = *((uint32_t *)prefetcher->data);
    uint32_t target = address+cache_system->line_size;
    for (int i=0; i < pAmount; i++){
        if (cache_system_mem_access(cache_system, target, 'R', true) == 0){
            count ++;
        }
        target += cache_system->line_size;
    }
    return count;
}

void sequential_cleanup(struct prefetcher *prefetcher)
{
    // TODO cleanup any additional memory that you allocated in the
    // sequential_prefetcher_new function.
}

struct prefetcher *sequential_prefetcher_new(uint32_t prefetch_amount)
{
    struct prefetcher *sequential_prefetcher = calloc(1, sizeof(struct prefetcher));
    sequential_prefetcher->handle_mem_access = &sequential_handle_mem_access;
    sequential_prefetcher->cleanup = &sequential_cleanup;

    // TODO allocate any additional memory needed to store metadata here and
    // assign to sequential_prefetcher->data.
    sequential_prefetcher->data = malloc(sizeof(uint32_t));
    *((uint32_t *) sequential_prefetcher->data) = prefetch_amount;

    return sequential_prefetcher;
}

// Adjacent Prefetcher
// ============================================================================
uint32_t adjacent_handle_mem_access(struct prefetcher *prefetcher,
                                    struct cache_system *cache_system, uint32_t address,
                                    bool is_miss)
{
    // TODO perform the necessary prefetches for the adjacent strategy.
    if (cache_system_mem_access(cache_system, address+cache_system->line_size, 'R', true) == 0){
        return 1;
    }

    // TODO: Return the number of lines that were prefetched.
    return 0;
}

void adjacent_cleanup(struct prefetcher *prefetcher)
{
    // TODO cleanup any additional memory that you allocated in the
    // adjacent_prefetcher_new function.
}

struct prefetcher *adjacent_prefetcher_new()
{
    struct prefetcher *adjacent_prefetcher = calloc(1, sizeof(struct prefetcher));
    adjacent_prefetcher->handle_mem_access = &adjacent_handle_mem_access;
    adjacent_prefetcher->cleanup = &adjacent_cleanup;

    // TODO allocate any additional memory needed to store metadata here and
    // assign to adjacent_prefetcher->data.

    return adjacent_prefetcher;
}

// Custom Prefetcher
// ============================================================================
uint32_t custom_handle_mem_access(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                  uint32_t address, bool is_miss)
{
    // TODO: Return the number of lines that were prefetched.
    uint32_t *adapt = *(uint32_t *) prefetcher->data;
    pAmount = 2;
    if (adapt[0] == 0){
//        cache_system_mem_access(cache_system, address+cache_system->line_size, 'R', true)
        printf("adapt = 0\n");
    } else {
        uint32_t index = (uint32_t) ((address - adapt[0])/cache_system->line_size);
        if (index >= 1 && index =< 7) {
            adapt[index] = adapt[index] + 1;
            printf("Index is: %d\n", index);
        } else {
            printf("Index doesn't fit. it is: %d\n", index);
        }
    }
//    int count = 0;
//    uint32_t target = address+cache_system->line_size;
//    for (int i=0; i < pAmount; i++){
//        if (cache_system_mem_access(cache_system, target, 'R', true) == 0){
//            count ++;
//        }
//        target += cache_system->line_size;
//    }
    adapt[0] = address;
    return count;
}

void custom_cleanup(struct prefetcher *prefetcher)
{
    // TODO cleanup any additional memory that you allocated in the
    // custom_prefetcher_new function.
}

struct prefetcher *custom_prefetcher_new()
{
    struct prefetcher *custom_prefetcher = calloc(1, sizeof(struct prefetcher));
    custom_prefetcher->handle_mem_access = &custom_handle_mem_access;
    custom_prefetcher->cleanup = &custom_cleanup;

    // TODO allocate any additional memory needed to store metadata here and
    // assign to custom_prefetcher->data.
    custom_prefetcher->data = calloc(7, sizeof(uint32_t));
    custom_prefetcher->data[0] = 0;


    return custom_prefetcher;
}
