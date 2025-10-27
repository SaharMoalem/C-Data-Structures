#include <stdlib.h>                  /* malloc, free */
#include <assert.h>                  /* assert */

#include "dl_list.h"

typedef struct dll_node_t {
    void* data;
    struct dll_node_t* next;
    struct dll_node_t* prev;
} dll_node_t;

struct dl_list {
    dll_node_t head;
    dll_node_t tail;
};

static dll_node_t* IterToNode(dll_iter_t iter)
{
    return iter;
}

static dll_iter_t NodeToIter(dll_node_t* node)
{
    assert(node);
    return node;
}

dl_list_t* DLListCreate(void)
{
    dl_list_t* p_dlist = (dl_list_t*)malloc(sizeof(dl_list_t));
    
    if(NULL == p_dlist)
    {
        return NULL;
    }

    p_dlist->head.data = NULL;
    p_dlist->tail.data = NULL;
    p_dlist->head.next = &p_dlist->tail;
    p_dlist->head.prev = NULL;
    p_dlist->tail.prev = &p_dlist->head;
    p_dlist->tail.next = NULL;

    return p_dlist;
}

void DLListDestroy(dl_list_t* dl_list)
{    
    assert(dl_list);
    
    while (!DLListIsEmpty(dl_list))
    {
        DLListPopFront(dl_list);
    }

    free(dl_list);
}

dll_iter_t DLListInsertBefore(dl_list_t* dl_list, dll_iter_t iter,
                                                            const void* element)
{
    dll_node_t* new_node = (dll_node_t*)malloc(sizeof(dll_node_t));
    dll_node_t* iter_node = IterToNode(iter);
    
    assert(dl_list);
    assert(element);

    if (NULL == new_node)
    {
        return DLListEnd(dl_list);
    }
    
    new_node->data = (void*)element;
    new_node->next = iter_node;
    new_node->prev = iter_node->prev;
    iter_node->prev->next = new_node;
    iter_node->prev = new_node;

    return NodeToIter(new_node);
}

dll_iter_t DLListPushFront(dl_list_t* dl_list, const void* element)
{
    return DLListInsertBefore(dl_list, DLListBegin(dl_list), element);
}

dll_iter_t DLListPushBack(dl_list_t* dl_list, const void* element)
{
    return DLListInsertBefore(dl_list, DLListEnd(dl_list), element);
}

dll_iter_t DLListRemoveElement(dll_iter_t iter)
{
    dll_node_t* iter_node = IterToNode(iter);
    dll_iter_t iter_node_next = NodeToIter(iter_node->next);
    
    iter_node->prev->next = iter_node->next;
    iter_node->next->prev = iter_node->prev;
    free(iter_node);
    
    return iter_node_next;
}

void* DLListPopFront(dl_list_t* dl_list)
{
    void* data;

    assert(dl_list);
    assert(!DLListIsEmpty(dl_list));
    
    data = DLListGetValue(DLListBegin(dl_list));
    DLListRemoveElement(DLListBegin(dl_list));
    
    return data;
}

void* DLListPopBack(dl_list_t* dl_list)
{
    dll_iter_t tail_prev = DLListPrev(DLListEnd(dl_list));
    void* data;
    assert(dl_list);
    assert(!DLListIsEmpty(dl_list));
    
    data = DLListGetValue(tail_prev);
    DLListRemoveElement(tail_prev);
    
    return data;
}

static int CountAction(void* data, void* param)
{
    (void)data;
    (*(size_t*)param)++;
    return 0;
}

size_t DLListSize(const dl_list_t* dl_list)
{
    size_t count = 0;
    
    assert(dl_list);

    DLListForEach(DLListBegin(dl_list), DLListEnd(dl_list), CountAction,
                                                                        &count);
    
    return count;
}

int DLListIsEmpty(const dl_list_t* dl_list)
{
    assert(dl_list);
    
    return dl_list->head.next == &dl_list->tail;
}

dll_iter_t DLListBegin(const dl_list_t* dl_list)
{
    assert(dl_list);
    
    return NodeToIter(dl_list->head.next);
}

dll_iter_t DLListEnd(const dl_list_t* dl_list)
{
    assert(dl_list);
    
    return NodeToIter((dll_iter_t)&dl_list->tail);
}

dll_iter_t DLListNext(dll_iter_t iter)
{
    return NodeToIter(IterToNode(iter)->next);
}

dll_iter_t DLListPrev(dll_iter_t iter)
{
    return NodeToIter(IterToNode(iter)->prev);
}

int DLListIsSameIter(dll_iter_t one, dll_iter_t other)
{
    return one == other;
}

void* DLListGetValue(dll_iter_t iter)
{
    return IterToNode(iter)->data;
}

void DLListSetValue(dll_iter_t iter, const void* value)
{
    assert(value);

    iter->data = (void*)value;
}

dll_iter_t DLListFind(dll_iter_t from, dll_iter_t to,
        int (*is_match)(const void* data, const void* param) ,const void* param)
{
    assert(is_match);
    
    while(!DLListIsSameIter(from, to))
    {
        if(is_match(IterToNode(from)->data, param))
        {
            break;
        }

	    from = from->next;
    }
	
    return from;
}

int DLListForEach(dll_iter_t from, dll_iter_t to,
                    int (*action_func)(void* data, void* param), void* param)
{
    int action_result;

    assert(action_func);

    while(!DLListIsSameIter(from, to))
    {
        action_result = action_func(IterToNode(from)->data, param);

        if(0 != action_result)
        {
            return action_result;
        }

	    from = from->next;
    }
	
    return 0;
}

int DLListMultiFind(dll_iter_t from, dll_iter_t to,
                    int (*is_match)(const void* data, const void* param)
                                    ,const void* param, dl_list_t* out_dl_list)
{
    dll_iter_t check_push_back;

    assert(out_dl_list);
    assert(is_match);
    
    while(!DLListIsSameIter(from, to))
    {
        if(is_match(IterToNode(from)->data, param))
	    {
	        check_push_back = DLListPushBack(out_dl_list, from->data);

	        if (DLListIsSameIter(check_push_back, DLListEnd(out_dl_list)))
	        {
	            return -1;
	        }
	    }
        
	    from = from->next;
    }
	
    return 0;
}

dll_iter_t DLListSplice(dll_iter_t from_src, dll_iter_t to_src,
                                                            dll_iter_t where)
{
    dll_node_t* from = IterToNode(from_src);
    dll_node_t* to = IterToNode(to_src);
    dll_node_t* dest = IterToNode(where);
    
    assert(from);
    assert(to);
    assert(dest);

    dest->next->prev = to->prev;
    to->prev->next = dest->next;
    dest->next = from;
    to->prev = from->prev;
    from->prev->next = to_src;
    from->prev = dest;
    
    return NodeToIter(dest);
}
