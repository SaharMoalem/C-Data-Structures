#include <stdlib.h>            /* malloc, free */
#include <assert.h>            /* assert */
#include <string.h>            /* memcpy */

#include "circular_buffer.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))

struct circular_buffer {
    size_t capacity;
    size_t size;
    size_t read;
    char array[1];
};

circular_buffer_t* CBCreate(size_t capacity)
{
    circular_buffer_t* c_buffer;
    
    assert(capacity > 0);

    c_buffer = (circular_buffer_t*)malloc(offsetof(circular_buffer_t, array) + 
			                                                        capacity);

    if (NULL == c_buffer)
    {
        return NULL;
    }

    c_buffer->capacity = capacity;
    c_buffer->size = 0;
    c_buffer->read = 0;
    
    return c_buffer;
}

void CBDestroy(circular_buffer_t* c_buffer)
{
    free(c_buffer);
}

size_t CBGetSize(const circular_buffer_t* c_buffer)
{
    assert(c_buffer);

    return c_buffer->size;
}

size_t CBGetCapacity(const circular_buffer_t* c_buffer)
{
    assert(c_buffer);

    return c_buffer->capacity;
}

size_t CBFreeSpace(const circular_buffer_t* c_buffer)
{
    assert(c_buffer);

    return c_buffer->capacity - c_buffer->size;
}

int CBIsEmpty(const circular_buffer_t* c_buffer)
{
    assert(c_buffer);

    return (0 == c_buffer->size);
}

ssize_t CBWrite(circular_buffer_t* c_buffer, const void* src, size_t bytes)
{
    size_t offset = offsetof(circular_buffer_t, array);
    size_t remain_bytes = 0;
    size_t write = 0;
    
    assert(c_buffer);
    assert(src);
    
    bytes = MIN(bytes, CBFreeSpace(c_buffer));
    write = (c_buffer->read + c_buffer->size) % c_buffer->capacity;
    remain_bytes = MIN(bytes, c_buffer->capacity - write);
    memcpy((unsigned char*)c_buffer + offset + write, src, remain_bytes);
    memcpy((unsigned char*)c_buffer + offset, (char*)src + remain_bytes, bytes -
                                                                remain_bytes);
    c_buffer->size += bytes;
    
    return bytes;
}

ssize_t CBRead(circular_buffer_t* c_buffer, void* dst, size_t bytes)
{
    size_t offset = offsetof(circular_buffer_t, array);
    size_t remain_bytes = 0;
    
    assert(c_buffer);
    assert(dst);
    
    bytes = MIN(bytes, c_buffer->size);
    remain_bytes = MIN(bytes, c_buffer->capacity - c_buffer->read);
    memcpy(dst, (unsigned char*)c_buffer + offset + c_buffer->read, remain_bytes);
    memcpy((char*)dst + remain_bytes, (unsigned char*)c_buffer + offset, bytes -
                                                                remain_bytes);
    c_buffer->read = (c_buffer->read + bytes) % c_buffer->capacity;
    c_buffer->size -= bytes;
    
    return bytes;
}
