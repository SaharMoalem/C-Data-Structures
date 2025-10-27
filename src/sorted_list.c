#include <stdlib.h>                  /* malloc, free */
#include <assert.h>                  /* assert */

#include "sorted_list.h"

struct sorted_list
{
    dl_list_t* list;
    int (*CompareFunc)(const void* ,const void*);
};

static dll_iter_t FindInsertLocation(sorted_list_t* sorted_list, dll_iter_t runner, void* param)
{
	while (!DLListIsSameIter(runner, DLListEnd(sorted_list->list)))
	{
		if (0 <= (sorted_list->CompareFunc(DLListGetValue(runner), param)))
		{
			break;
		}

		runner = DLListNext(runner);
	}

	return runner;	
}


sorted_list_t* CreateSortedList(int (*CompareFunc)(const void* ,const void*))
{
    sorted_list_t* sorted_list;
    
    assert(CompareFunc);

    sorted_list = (sorted_list_t*)malloc(sizeof(sorted_list_t));

    if (NULL == sorted_list)
    {
        return NULL;
    }

    sorted_list->list = DLListCreate();

    if(NULL == sorted_list->list)
    {
        free(sorted_list);
        return NULL;
    }
    
    sorted_list->CompareFunc = CompareFunc;
    
    return sorted_list;
}

void SortedListDestroy(sorted_list_t* sorted_list)
{
    assert(sorted_list);
    
    DLListDestroy(sorted_list->list);
    free(sorted_list);
}

sorted_list_iter_t SortedListInsert(sorted_list_t* sorted_list, void* data)
{
    sorted_list_iter_t iter;

	assert(NULL != sorted_list);

	iter = SortedListBegin(sorted_list);
	iter.internal_iter = FindInsertLocation(sorted_list, 
										DLListBegin(sorted_list->list), data);
	iter.internal_iter = DLListInsertBefore(sorted_list->list, 
										iter.internal_iter, (const void*)data);

	return iter;
}

sorted_list_iter_t SortedListRemove(sorted_list_iter_t where)
{
    where.internal_iter = DLListRemoveElement(where.internal_iter);

    return where;
}

void* SortedListPopFront(sorted_list_t* sorted_list)
{
    assert(sorted_list);

    return DLListPopFront(sorted_list->list);
}

void* SortedListPopBack(sorted_list_t* sorted_list)
{
    assert(sorted_list);

    return DLListPopBack(sorted_list->list);
}

size_t SortedListSize(const sorted_list_t* sorted_list)
{
    assert(sorted_list);

    return DLListSize(sorted_list->list);
}

int SortedListIsEmpty(const sorted_list_t* sorted_list)
{
    assert(sorted_list);

    return DLListIsEmpty(sorted_list->list);
}

sorted_list_iter_t SortedListBegin(sorted_list_t* sorted_list)
{
    sorted_list_iter_t begin_iter;
    
    assert(sorted_list);

    begin_iter.internal_iter = DLListBegin(sorted_list->list);

    #ifndef NDEBUG
        begin_iter.list = sorted_list;
    #endif

    return begin_iter;
}

sorted_list_iter_t SortedListEnd(sorted_list_t* sorted_list)
{
    sorted_list_iter_t end_iter;

    end_iter.internal_iter = DLListEnd(sorted_list->list);

    #ifndef NDEBUG
        end_iter.list = sorted_list;
    #endif

    return end_iter;
}

sorted_list_iter_t SortedListNext(sorted_list_iter_t iter)
{
    iter.internal_iter = DLListNext(iter.internal_iter);

    return iter;
}

sorted_list_iter_t SortedListPrev(sorted_list_iter_t iter)
{
    iter.internal_iter = DLListPrev(iter.internal_iter);

    return iter;
}

int SortedListIsSameIter(sorted_list_iter_t one, sorted_list_iter_t other)
{
    return DLListIsSameIter(one.internal_iter, other.internal_iter);
}

const void* SortedListGetValue(sorted_list_iter_t iter)
{
    return DLListGetValue(iter.internal_iter);
}

sorted_list_iter_t SortedListFind(sorted_list_t* list, sorted_list_iter_t from, sorted_list_iter_t to, const void* param)
{    
    assert(list);
    assert(from.list == to.list);
    assert(from.list == list);
    
    for( ; !SortedListIsSameIter(from, to); from = SortedListNext(from))
    {
    	if(0 == list->CompareFunc(SortedListGetValue(from), param))
    	{
    	    return from;
    	}
    }
    
    return to;
}

sorted_list_iter_t SortedListFindIf(sorted_list_iter_t from, sorted_list_iter_t to, int (*is_match)(const void* data, const void* param) ,const void* param)
{
    assert(is_match);
    assert(from.list == to.list);
    
    from.internal_iter = DLListFind(from.internal_iter, to.internal_iter, is_match, param);

    return from;
}

int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to, int (*action_func)(void* data, void* param), void* param)
{
    assert(action_func);
    assert(from.list == to.list);
    
    return DLListForEach(from.internal_iter, to.internal_iter, action_func, param);
}

static sorted_list_iter_t FindWhereToMerge(sorted_list_t* list, sorted_list_iter_t iter, const void* data)
{
    while (!SortedListIsSameIter(iter, SortedListEnd(list)) && 0 > list->CompareFunc(SortedListGetValue(iter), data))
    {
        iter = SortedListNext(iter);
    }

    return iter;
}

void SortedListMerge(sorted_list_t* list_dst, sorted_list_t* list_src)
{
    sorted_list_iter_t dst_iter = SortedListBegin(list_dst);
    sorted_list_iter_t src_start;
    sorted_list_iter_t src_end;
    
    assert(list_dst);
    assert(list_src);
    assert(list_src != list_dst);
    assert(list_src->CompareFunc == list_dst->CompareFunc);
		
    while (!SortedListIsEmpty(list_src))
    {
	    src_start = SortedListBegin(list_src);
	    src_end = src_start;
	    dst_iter = FindWhereToMerge(list_dst, dst_iter,
                                                SortedListGetValue(src_start));

    	if (SortedListIsSameIter(dst_iter, SortedListEnd(list_dst)))
      	{
      	    src_end = SortedListEnd(list_src);
	    }

	    else
	    {
	        src_end = FindWhereToMerge(list_src, src_end,
                                                SortedListGetValue(dst_iter));
	    }

	    DLListSplice(src_start.internal_iter, src_end.internal_iter,
                                        SortedListPrev(dst_iter).internal_iter);
    }
}
