#include <assert.h>      /*assert*/
#include <stdlib.h>      /*malloc, free*/

#include "hash_table.h"
#include "dl_list.h"

struct hash_table {
    size_t n_buckets;
    hash_func_t hash_func;
    is_match_t match_func;
    dl_list_t** buckets;
};

static void DestroyTable(hash_table_t* hash_table, size_t lists)
{
    size_t i = 0;

    for( ; i < lists; ++i)
    {
        DLListDestroy(hash_table->buckets[i]);
    }

    free(hash_table->buckets);
    free(hash_table);
}

static size_t GetHashIndex(hash_table_t* hash_table, const void* data)
{
    return hash_table->hash_func(data) % hash_table->n_buckets;
}

hash_table_t* HTCreate(size_t n_buckets, hash_func_t hash_func,
                                                        is_match_t match_func)
{
    hash_table_t* hash_table = NULL;
    size_t i = 0;
    
    assert(hash_func);
    assert(match_func);
    assert(n_buckets);

    hash_table = (hash_table_t*)malloc(sizeof(hash_table_t));

    if(!hash_table)
    {
        return NULL;
    }

    hash_table->buckets = (dl_list_t**)malloc(sizeof(dl_list_t*) * n_buckets);

    if(!hash_table->buckets)
    {
        free(hash_table);
        return NULL;
    }

    for( ; i < n_buckets; ++i)
    {
        hash_table->buckets[i] = DLListCreate();

        if(!hash_table->buckets[i])
        {
            DestroyTable(hash_table, i);
            return NULL;
        }
    }

    hash_table->n_buckets = n_buckets;
    hash_table->hash_func = hash_func;
    hash_table->match_func = match_func;

    return hash_table;
}

void HTDestroy(hash_table_t* hash_table)
{
    assert(hash_table);

    DestroyTable(hash_table, hash_table->n_buckets);
}

int HTInsert(hash_table_t* hash_table, const void* data)
{
    dl_list_t* list = NULL;
    dll_iter_t iter;

    assert(hash_table);

    list = hash_table->buckets[GetHashIndex(hash_table, data)];
    iter = DLListFind(DLListBegin(list), DLListEnd(list),
                                                hash_table->match_func, data);

    if(!DLListIsSameIter(iter, DLListEnd(list)))
    {
        DLListRemoveElement(iter);
    }

    return !DLListIsSameIter(DLListPushBack(list, data), DLListEnd(list));
}

void HTRemove(hash_table_t* hash_table, const void* data)
{
    dl_list_t* list = NULL;
    dll_iter_t iter;

    assert(hash_table);

    list = hash_table->buckets[GetHashIndex(hash_table, data)];
    iter = DLListFind(DLListBegin(list), DLListEnd(list),
                                                hash_table->match_func, data);
                                                
    if(!DLListIsSameIter(iter, DLListEnd(list)))
    {
        DLListRemoveElement(iter);
    }
}

int HTIsEmpty(const hash_table_t* hash_table)
{
    size_t i = 0;

    assert(hash_table);

    for( ; i < hash_table->n_buckets * DLListIsEmpty(hash_table->buckets[i]);
                                                                        ++i);

    return i == hash_table->n_buckets;
}

size_t HTGetSize(const hash_table_t* hash_table)
{
    size_t count = 0;
    size_t i = 0;

    assert(hash_table);

    for( ; i < hash_table->n_buckets; ++i)
    {
        count += DLListSize(hash_table->buckets[i]);
    }

    return count;
}

void* HTFind(const hash_table_t* hash_table, const void* data)
{
    dl_list_t* list = NULL;
    dll_iter_t iter;

    assert(hash_table);

    list = hash_table->buckets[GetHashIndex((hash_table_t*)hash_table, data)];
    iter = DLListFind(DLListBegin(list), DLListEnd(list),
                                                hash_table->match_func, data);

    return !DLListIsSameIter(iter, DLListEnd(list)) ? DLListGetValue(iter) :
                                                                        NULL;
}

int HTForeach(const hash_table_t* hash_table, action_func_t action_func, 
                                                                    void* param)
{
    dl_list_t* list = NULL;
    size_t i = 0;
    int action_result = 0;

    assert(hash_table);
    assert(action_func);

    for( ; i < hash_table->n_buckets; ++i)
    {
        list = hash_table->buckets[i];
        action_result = DLListForEach(DLListBegin(list), DLListEnd(list),
                                                            action_func, param);
        if(action_result != 0)
        {
            return action_result;
        }
    }

    return 0;
}

double HTLoadFactor(const hash_table_t* hash_table)
{
    assert(hash_table);

    return (double)HTGetSize(hash_table) / hash_table->n_buckets;
}

static double SquareRoot(double number)
{
    double temp = 0;
    double sqrt = number / 2;
    
    while (sqrt != temp)
    {
        temp = sqrt;
        sqrt = (sqrt + number / sqrt) / 2;
    }
    
    return sqrt;
}

double HTGetSD(const hash_table_t* hash_table)
{
    double average = 0;
    double total = 0;
    double total_curr;
    size_t i = 0;

    assert(hash_table);

    average = HTLoadFactor(hash_table);
    for( ; i < hash_table->n_buckets; ++i)
    {
        total_curr = DLListSize(hash_table->buckets[i]) - average;
        total += total_curr * total_curr;
    }

    return SquareRoot(total / hash_table->n_buckets);
}
