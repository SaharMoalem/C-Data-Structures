#include <stdlib.h>                  /* malloc, free */
#include <assert.h>                  /* assert */

#include "sl_list.h"

struct sll_node {
    void* data;
    sll_node_t* next;
};

struct sl_list {
    sll_node_t* head;
    sll_node_t* tail;
};

static sll_node_t* IterToNode(iterator_t iter)
{
    return iter;
}

static iterator_t NodeToIter(sll_node_t* node)
{
    return node;
}

static iterator_t FindEnd(iterator_t iter)
{
    while (NULL != iter->next)
    {
        iter = iter->next;
    }

    return iter;
}

sl_list_t* SLListCreate(void)
{
    sl_list_t* p_list = (sl_list_t*)malloc(sizeof(sl_list_t));
    sll_node_t* dummy;

    if (NULL == p_list)
    {
        return NULL;
    }

    dummy = (sll_node_t*)malloc(sizeof(sll_node_t));

    if (NULL == dummy)
    {
        free(p_list);
        return NULL;
    }

    dummy->data = p_list;
    dummy->next = NULL;
    p_list->head = dummy;
    p_list->tail = dummy;

    return p_list;
}

void SLListDestroy(sl_list_t* sl_list)
{    
    assert(NULL != sl_list);

    while (!SLListIsEmpty(sl_list))
    {
        SLListRemoveElement(SLListBegin(sl_list));
    }

    free(sl_list->tail);
    free(sl_list);
}

iterator_t SLListInsertBefore(iterator_t iter, const void* element)
{
    sll_node_t* new_node = (sll_node_t*)malloc(sizeof(sll_node_t));
    sll_node_t* iter_node = IterToNode(iter);
    void* tmp_data = iter_node->data;
    iterator_t tmp_next = iter_node->next;
    
    if(NULL == new_node)
    {
        return FindEnd(iter);
    }
    
    iter_node->data = (void*)element;
    iter_node->next = new_node;
    new_node->data = tmp_data;
    new_node->next = tmp_next;
    if (NULL == new_node->next)
    {
        ((sl_list_t*)new_node->data)->tail = new_node;
    }
    
    return iter_node;
}

iterator_t SLListRemoveElement(iterator_t iter)
{
    sll_node_t* iter_curr = IterToNode(iter);
    sll_node_t* iter_next = iter_curr->next;
    
    iter_curr->data = iter_next->data;
    iter_curr->next = iter_next->next;

    if (NULL == iter_curr->next)
    {
        ((sl_list_t*)iter_curr->data)->tail = iter_curr;
    }
    
    free(iter_next);

    return NodeToIter(iter_curr);
}

size_t SLListCount(const sl_list_t* sl_list)
{
    iterator_t curr = sl_list->head;
    size_t count = 0;
    
    assert(NULL != sl_list);

    while(sl_list->tail != curr)
    {
        ++count;
        curr = curr->next;
    }
    
    return count;
}

int SLListIsEmpty(const sl_list_t* sl_list)
{
    assert(NULL != sl_list);
    
    return 0 == SLListCount(sl_list);
}

iterator_t SLListBegin(const sl_list_t* sl_list)
{
    assert(NULL != sl_list);
    
    return NodeToIter(sl_list->head);
}

iterator_t SLListEnd(const sl_list_t* sl_list)
{
    assert(NULL != sl_list);
    
    return NodeToIter(sl_list->tail);
}

iterator_t SLListNext(iterator_t iter)
{
    return NodeToIter(IterToNode(iter)->next);
}

int SLListIsSameIter(iterator_t one, iterator_t other)
{
    sll_node_t* one_node = IterToNode(one);
    sll_node_t* other_node = IterToNode(other);
    
    return one_node->data == other_node->data && one_node->next == other_node->next;
}

void* SLListGetValue(iterator_t iter)
{
    return IterToNode(iter)->data;
}

void SLListSetValue(iterator_t iter, const void* value)
{
    IterToNode(iter)->data = (void*)value;
}

iterator_t SLListFind(iterator_t from, iterator_t to, int (*is_match)(const void* data, const void* element) ,const void* element)
{
    assert(NULL != is_match);

    while(!SLListIsSameIter(from, to))
    {
        if(is_match(IterToNode(from)->data, element))
        {
            return from;
        }

        from = from->next;
    }
	
    return FindEnd(from);
}

iterator_t SLListForEach(iterator_t from, iterator_t to, int (*action_func)(void* data, void* param), void* param)
{
    assert(NULL != action_func);
    
    while(!SLListIsSameIter(from, to))
    {
        if(0 != action_func(IterToNode(from)->data, param))
        {
            return from;
        }

        from = from->next;
    }
	
    return to;
}

void SLListAppend(sl_list_t* sl_src, sl_list_t* sl_dst)
{
    iterator_t dst_tail;
    iterator_t src_tail;
    iterator_t src_head;
    
    assert(sl_src);
    assert(sl_dst);
    
    dst_tail = SLListEnd(sl_dst);
    src_tail = SLListEnd(sl_src);
    src_head = SLListBegin(sl_src);
    
    dst_tail->data = src_head->data;
    dst_tail->next = src_head->next;
    src_tail->data = sl_dst;
    src_head->data = sl_src;
    src_head->next = NULL;
    sl_src->tail = src_head;
    sl_dst->tail = src_tail;
}
