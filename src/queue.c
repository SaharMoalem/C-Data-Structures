#include <stdlib.h>                  /* malloc, free */
#include <assert.h>                  /* assert */

#include "queue.h"
#include "sl_list.h"

struct queue {
    sl_list_t* list;
};

queue_t* QCreate(void)
{
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));

    if (NULL == queue)
    {
        return NULL;
    }
    queue->list = SLListCreate();

    if (NULL == queue->list)
    {
        free(queue);
        return NULL;
    }
    
    return queue;
}

void QDestroy(queue_t* queue)
{
    assert(queue);

    SLListDestroy(queue->list);
    free(queue);
}

size_t QGetSize(const queue_t* queue)
{
    assert(queue);

    return SLListCount(queue->list);
}

int QIsEmpty(const queue_t* queue)
{
    assert(queue);

    return 0 == QGetSize(queue);
}

void* QPeek(const queue_t* queue)
{
    assert(queue);

    return SLListGetValue(SLListBegin(queue->list));
}

int QEnqueue(queue_t* queue, const void* data)
{
    assert(queue);
    assert(data);

    return SLListIsSameIter(SLListInsertBefore(SLListEnd(queue->list), data), SLListEnd(queue->list));
}

void QDequeue(queue_t* queue)
{
    assert(queue);
    assert(!QIsEmpty(queue));
    
    SLListRemoveElement(SLListBegin(queue->list));
}

void QAppend(queue_t* src, queue_t* dst)
{
    SLListAppend(src->list, dst->list);
}
