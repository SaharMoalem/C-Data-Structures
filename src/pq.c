#include <stdlib.h>                  /* malloc, free */
#include <assert.h>                  /* assert */

#include "pq.h"
#include "sorted_list.h"

struct priority_queue
{
    sorted_list_t* list;
};

pq_t* PQCreate(int (*compare_func)(const void*, const void*))
{
    pq_t* pq = NULL;
    
    assert(compare_func);

    pq = (pq_t*)malloc(sizeof(pq_t));

    if (pq == NULL)
    {
        return NULL;
    }
    
    pq->list = CreateSortedList(compare_func);
    
    if (pq->list == NULL)
    {
        free(pq);
        return NULL;
    }
    
    return pq;
}

void PQDestroy(pq_t* pq)
{
    assert(pq);
    
    SortedListDestroy(pq->list);
    free(pq);
}

int PQEnqueue(pq_t* pq, void* data)
{
    assert(pq);
    
    return SortedListIsSameIter(SortedListEnd(pq->list), SortedListInsert(pq->list, data));
}

void* PQDequeue(pq_t* pq)
{
    assert(pq);
    assert(!PQIsEmpty(pq));
    
    return SortedListPopFront(pq->list);
}

void* PQPeek(const pq_t* pq)
{
    assert(pq);
    assert(!PQIsEmpty(pq));
    
    return (void*)SortedListGetValue(SortedListBegin(pq->list));
}

int PQIsEmpty(const pq_t* pq)
{
    assert(pq);
    
    return SortedListIsEmpty(pq->list);
}

size_t PQSize(const pq_t* pq)
{
    assert(pq);
    
    return SortedListSize(pq->list);
}

void PQClear(pq_t* pq)
{
    assert(pq);
    
    while (!PQIsEmpty(pq))
    {
        PQDequeue(pq);
    }
}

void* PQErase(pq_t* pq, int (*is_match)(const void*, const void*), const void* param)
{
    sorted_list_iter_t find;
    void* data = NULL;
    assert(pq);
    assert(is_match);
    
    find = SortedListFindIf(SortedListBegin(pq->list),
                                      SortedListEnd(pq->list), is_match, param);
       
    if (SortedListIsSameIter(find, SortedListEnd(pq->list)))
    {
        return NULL;
    }
    
    data = (void*)SortedListGetValue(find);
    SortedListRemove(find);
    
    return data;
}
