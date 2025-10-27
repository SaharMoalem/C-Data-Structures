#include <assert.h>
#include "fsa.h"

#define WORD_SIZE (sizeof(size_t))

struct fsa
{
    size_t next_free;
};

typedef struct header
{
    size_t next;
} header_t;

static size_t AlignBlock(size_t block_size)
{
    return ((block_size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE;
}

size_t FSASuggestSize(size_t block_size, size_t n_blocks)
{
    block_size = AlignBlock(block_size);

    return (block_size + sizeof(header_t)) * n_blocks + sizeof(fsa_t);
}

fsa_t* FSAInit(void* memory, size_t block_size, size_t memory_size)
{
    fsa_t* fsa = NULL;
    header_t* header = NULL;
    header_t* header_next = NULL;

    assert(memory);

    block_size = AlignBlock(block_size);

    if (block_size + sizeof(fsa_t) + sizeof(header_t) > memory_size)
    {
        return NULL;
    }

    fsa = memory;
    fsa->next_free = sizeof(fsa_t);
    header = (header_t*)((char*)memory + fsa->next_free);
    header->next = sizeof(fsa_t) + sizeof(header_t) + block_size;

    while(header->next <= memory_size - block_size - sizeof(header_t))
    {
        header_next = (header_t*)((char*)memory + header->next);
        header_next->next = header->next + sizeof(header_t) + block_size;
        header = header_next;
    }

    header->next = 0;

    return fsa;
}

void* FSAAlloc(fsa_t* fsa)
{
    header_t* header = NULL;
    
    assert(fsa);

    if(fsa->next_free == 0)
    {
        return NULL;
    }

    header = (header_t*)((char*)fsa + fsa->next_free);
    fsa->next_free = header->next;

    return header + 1;
}

void FSAFree(fsa_t* fsa, void* p_block)
{
    header_t* header = NULL;

    assert(fsa);
    assert(p_block);

    header = (header_t*)p_block - 1;
    header->next = fsa->next_free;
    fsa->next_free = (size_t)header - (size_t)fsa;
}

size_t FSACountFree(const fsa_t* fsa)
{
    header_t* header = NULL;
    size_t count = 0;
    size_t offset = 0;

    assert(fsa);

    offset = fsa->next_free;
    
    while(offset != 0)
    {
        header = (header_t*)((char*)fsa + offset);
        offset = header->next;
        ++count;
    }

    return count;
}

