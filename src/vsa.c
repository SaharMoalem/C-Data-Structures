#include <assert.h>

#include "vsa.h"

#define WORD_SIZE (sizeof(size_t))
#define HEADER_SIZE (sizeof(block_header_t))
#define ABS(num) ((num < 0) ? -num : num)
#define MAGIC_NUMBER (0xCAFEBABE)

typedef struct block_header
{
    long block_size;
#ifndef NDEBUG
    size_t magic_num;
#endif
} block_header_t;

struct vsa
{
    block_header_t first_header;
};

static size_t AlignBlock(size_t block_size)
{
    return ((block_size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE;
}

static block_header_t* GetNextHeader(block_header_t* header)
{
    return (block_header_t*)((char*)(header + 1) + ABS(header->block_size));
}

static int IsFree(block_header_t* header)
{
    return header->block_size > 0;
}

static void Defrag(block_header_t* header)
{
    block_header_t* header_runner = NULL;

    if(!IsFree(header))
    {
        return;
    }

    header_runner = GetNextHeader(header);

    while(IsFree(header_runner))
    {
        header->block_size += header_runner->block_size + HEADER_SIZE;
        header_runner = GetNextHeader(header_runner);
    }
}

static void* AllocBlock(block_header_t* header, size_t alloc_size)
{
    block_header_t* new_header = NULL;

    if(header->block_size > (long)(alloc_size + HEADER_SIZE))
    {
        new_header = (block_header_t*)((char*)(header) + alloc_size
                                                       + HEADER_SIZE);
        new_header->block_size = header->block_size - alloc_size - HEADER_SIZE;
        header->block_size = -(long)alloc_size;        
    }
    else
    {
        header->block_size *= -1;
    }

#ifndef NDEBUG
    header->magic_num = MAGIC_NUMBER;      
#endif

    return header + 1;
}


vsa_t* VSAInit(void* memory, size_t memory_size)
{
    vsa_t* vsa = NULL;
    block_header_t* last_header  = NULL;

    assert(memory);
    
    if(memory_size <= 2 * HEADER_SIZE)
    {
        return NULL;
    }

    vsa = (vsa_t*)memory;
    vsa->first_header.block_size = memory_size - 2 * HEADER_SIZE;
    last_header = GetNextHeader(&vsa->first_header);
    last_header->block_size = 0;

#ifndef NDEBUG
        vsa->first_header.magic_num = MAGIC_NUMBER;
        last_header->magic_num = MAGIC_NUMBER;
#endif

    return vsa;
}

void* VSAAlloc(vsa_t* vsa, size_t alloc_size)
{
    block_header_t* header = NULL;

    assert(vsa);

    if(alloc_size == 0)
    {
        return NULL;
    }

    alloc_size = AlignBlock(alloc_size);
    header = &vsa->first_header;
    while(header->block_size != 0) 
    {
        Defrag(header);
        if(header->block_size >= (long)alloc_size)
        {
            return AllocBlock(header, alloc_size);
        }

        header = GetNextHeader(header);
    }

    return NULL;
}

void VSAFree(void* block)
{
    block_header_t* header = NULL;

    if(block)
    {
        header = (block_header_t*)block - 1;
        assert(header->magic_num == MAGIC_NUMBER);
        header->block_size *= -1;
    }
}

size_t VSALargestChunkAvailable(vsa_t* vsa)
{
    block_header_t* header = NULL;
    long max_size = 0;

    assert(vsa);

    header = &vsa->first_header;
    while(header->block_size != 0)
    {
        Defrag(header);
        max_size = header->block_size > max_size ? header->block_size
                                                    : max_size;
        header = GetNextHeader(header);
    }

    return (size_t)max_size;
}