//
// This file contains all of the implementations of the replacement_policy
// constructors from the replacement_policies.h file.
//
// It also contains stubs of all of the functions that are added to each
// replacement_policy struct at construction time.
//
// ============================================================================
// NOTE: It is recommended that you read the comments in the
// replacement_policies.h file for further context on what each function is
// for.
// ============================================================================
//

#include "replacement_policies.h"

// LRU Replacement Policy
// ============================================================================
// TODO feel free to create additional structs/enums as necessary

struct mem{
    int lastUsed;
    bool active;
};


void lru_cache_access(struct replacement_policy *replacement_policy,
                      struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    replacement_policy->currentTime += 1;
    // TODO update the LRU replacement policy state given a new memory access
    // Is the tag in our data already?
    int index;
    for (int i=set_idx*cache_system->associativity;i<((set_idx*cache_system->associativity)+cache_system->associativity);i++){
        if (cache_system->cache_lines[i].tag == tag){
            index = i;
        }
    }
    struct mem *tagLoc = &replacement_policy->data[index];
    tagLoc->active = true;
    tagLoc->lastUsed = replacement_policy->currentTime;
}

uint32_t lru_eviction_index(struct replacement_policy *replacement_policy,
                            struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.
    // Remove lowest currentTime & active
    uint32_t highestCurrentTime = INT32_MAX;
    int bestIndex;
    for (int i=set_idx*cache_system->associativity; i<(set_idx+1)*cache_system->associativity;i++){
        struct mem *tagLoc = &replacement_policy->data[i];
        if (tagLoc->active && tagLoc->lastUsed < highestCurrentTime){
            bestIndex = i;
            highestCurrentTime = tagLoc->lastUsed;
        }
    }
    struct mem *tagLoc = &replacement_policy->data[bestIndex];
    tagLoc->active = false;
    tagLoc->lastUsed = INT32_MAX;
    return bestIndex - (set_idx*cache_system->associativity);

}

void lru_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_replacement_policy_new function.
//    free(replacement_policy->data);
}

struct replacement_policy *lru_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    struct replacement_policy *lru_rp = calloc(1, sizeof(struct replacement_policy));
    lru_rp->cache_access = &lru_cache_access;
    lru_rp->eviction_index = &lru_eviction_index;
    lru_rp->cleanup = &lru_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // lru_rp->data. (Here i probably need to store the oldest line or each line's last use.)
    lru_rp->data = (struct mem*) calloc(sets * associativity, sizeof(struct mem));
    lru_rp->size = 0;
    lru_rp->currentTime = 0;
    return lru_rp;
}

// RAND Replacement Policy
// ============================================================================
void rand_cache_access(struct replacement_policy *replacement_policy,
                       struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // TODO update the RAND replacement policy state given a new memory access

}

uint32_t rand_eviction_index(struct replacement_policy *replacement_policy,
                             struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.
    int max = ((set_idx+1) * cache_system->associativity)-1;
    int min = set_idx*cache_system->associativity;
    int range = max-min-1;
    int random_number = rand() % range + min;
    return random_number - (set_idx*cache_system->associativity);
}

void rand_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // rand_replacement_policy_new function.
//    free(replacement_policy->data);
}

struct replacement_policy *rand_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    // Seed randomness
    srand(time(NULL));

    struct replacement_policy *rand_rp = malloc(sizeof(struct replacement_policy));
    rand_rp->cache_access = &rand_cache_access;
    rand_rp->eviction_index = &rand_eviction_index;
    rand_rp->cleanup = &rand_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // rand_rp->data.


    return rand_rp;
}

// LRU_PREFER_CLEAN Replacement Policy
// ============================================================================
void lru_prefer_clean_cache_access(struct replacement_policy *replacement_policy,
                                   struct cache_system *cache_system, uint32_t set_idx,
                                   uint32_t tag)
{
    // TODO update the LRU_PREFER_CLEAN replacement policy state given a new
    // memory access
    // NOTE: you may be able to share code with the LRU policy
    replacement_policy->currentTime += 1;
    // TODO update the LRU replacement policy state given a new memory access
    // Is the tag in our data already?
    int index;
    for (int i=set_idx*cache_system->associativity;i<((set_idx*cache_system->associativity)+cache_system->associativity);i++){
        if (cache_system->cache_lines[i].tag == tag){
            index = i;
        }
    }
    struct mem *tagLoc = &replacement_policy->data[index];
    tagLoc->active = true;
    tagLoc->lastUsed = replacement_policy->currentTime;
}

uint32_t lru_prefer_clean_eviction_index(struct replacement_policy *replacement_policy,
                                         struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.
    // TODO return the index within the set that should be evicted.
    // Remove lowest currentTime & active
    uint32_t highestCurrentTimeDirty = INT32_MAX;
    uint32_t highestCurrentTimeClean = INT32_MAX;
    int bestIndex;
    for (int i=set_idx*cache_system->associativity; i<(set_idx+1)*cache_system->associativity;i++){
        struct mem *tagLoc = &replacement_policy->data[i];
        if (cache_system->cache_lines[i].status == MODIFIED && highestCurrentTimeClean==INT32_MAX){ // Dirty dirty
            if (tagLoc->active && tagLoc->lastUsed < highestCurrentTimeDirty){
                bestIndex = i;
                highestCurrentTimeDirty = tagLoc->lastUsed;
            }
        } else if (cache_system->cache_lines[i].status != MODIFIED){                                            // Clean (Which is prefered)
            if (tagLoc->active && tagLoc->lastUsed < highestCurrentTimeClean){
                bestIndex = i;
                highestCurrentTimeClean = tagLoc->lastUsed;
            }
        }

    }
    struct mem *tagLoc = &replacement_policy->data[bestIndex];
    tagLoc->active = false;
    tagLoc->lastUsed = INT32_MAX;
    return bestIndex - (set_idx*cache_system->associativity);
}

void lru_prefer_clean_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_prefer_clean_replacement_policy_new function.
}

struct replacement_policy *lru_prefer_clean_replacement_policy_new(uint32_t sets,
                                                                   uint32_t associativity)
{
    struct replacement_policy *lru_prefer_clean_rp = malloc(sizeof(struct replacement_policy));
    lru_prefer_clean_rp->cache_access = &lru_prefer_clean_cache_access;
    lru_prefer_clean_rp->eviction_index = &lru_prefer_clean_eviction_index;
    lru_prefer_clean_rp->cleanup = &lru_prefer_clean_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // lru_prefer_clean_rp->data.
    lru_prefer_clean_rp->data = (struct mem*) calloc(sets * associativity, sizeof(struct mem));
    lru_prefer_clean_rp->currentTime = 0;


    return lru_prefer_clean_rp;
}
