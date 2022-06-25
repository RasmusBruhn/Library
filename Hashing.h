#ifndef HASHING_H_INCLUDED
#define HASHING_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <Random.h>

#define ERR_PREFIX HAS
#include <Error.h>

enum _HAS_ErrorID {
    _HAS_ERRORID_NONE = 0x500000000,
    _HAS_ERRORID_CREATEHASH_MALLOCHASH = 0x500010200,
    _HAS_ERRORID_CREATEHASH_MALLOCTABLE = 0x500010201
};

#define _HAS_ERRORMES_MALLOC "Unable to allocate memory (Size: %lu)"

typedef struct __HAS_Hash HAS_Hash;

struct __HAS_Hash {
    uint64_t *table;
    uint32_t depth;
};

#define _HAS_HASHBASESIZE 256

// Create a hash 
HAS_Hash *HAS_CreateHash(uint32_t Depth, uint64_t Seed);

// Destroy a hash
void HAS_DestroyHash(HAS_Hash *Hash);

// Hash a value
uint64_t HAS_HashValue(HAS_Hash *Hash, uint8_t *Value, size_t Length);

HAS_Hash *HAS_CreateHash(uint32_t Depth, uint64_t Seed)
{
    // Allocate memory
    HAS_Hash *Hash = (HAS_Hash *)malloc(sizeof(HAS_Hash));

    if (Hash == NULL)
    {
        _HAS_AddErrorForeign(_HAS_ERRORID_CREATEHASH_MALLOCHASH, strerror(errno), _HAS_ERRORMES_MALLOC, sizeof(HAS_Hash));
        return NULL;
    }

    Hash->table = (uint64_t *)malloc(sizeof(uint64_t) * (_HAS_HASHBASESIZE * Depth));

    if (Hash->table == NULL)
    {
        _HAS_AddErrorForeign(_HAS_ERRORID_CREATEHASH_MALLOCTABLE, strerror(errno), _HAS_ERRORMES_MALLOC, sizeof(uint64_t) * (_HAS_HASHBASESIZE * Depth));
        free(Hash);
        return NULL;
    }

    // Set the depth
    Hash->depth = Depth;

    // Fill up the table
    for (uint16_t *List = (uint16_t *)Hash->table, *EndList = (uint16_t *)Hash->table + _HAS_HASHBASESIZE * Depth * 4; List < EndList; ++List)
        *List = (uint16_t)(RNG_RandS(Seed) % 0x10000);

    return Hash;
}

void HAS_DestroyHash(HAS_Hash *Hash)
{
    // Free the table
    if (Hash->table != NULL)
        free(Hash->table);

    free(Hash);
}

uint64_t HAS_HashValue(HAS_Hash *Hash, uint8_t *Value, size_t Length)
{
    // Go through all elements and hash them together
    uint64_t HashValue = 0;
    uint32_t Layer = 0;

    for (uint8_t *List = Value, *EndList = Value + Length; List < EndList; ++List)
    {
        HashValue ^= *(Hash->table + _HAS_HASHBASESIZE * Layer++ + *List);
        
        // Loop the layer
        if (Layer == Hash->depth)
            Layer = 0;
    }

    return HashValue;
}

#endif