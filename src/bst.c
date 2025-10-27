#include <stdlib.h>     /* malloc, free */
#include <assert.h>     /* assert */

#include "bst.h"

struct bst_node
{
    void* data;
    struct bst_node* left;
    struct bst_node* right;
    struct bst_node* parent;
};

struct bst
{
    compare_func_t compare_func;
    bst_node_t dummy;
};

static bst_node_t* IterToNode(bst_iter_t iter)
{
    return iter.node;
}

static bst_iter_t NodeToIter(bst_node_t* node)
{
    bst_iter_t iter;

    iter.node = node;

    return iter;
}

static bst_iter_t PlaceNode(bst_t* bst, bst_node_t* node)
{
    bst_node_t* runner_prev = &bst->dummy;
    bst_node_t** runner = &(runner_prev->left);
    int compare_result = 0;

    while(*runner != NULL)
    {
        runner_prev = *runner;
        compare_result = bst->compare_func(node->data, (*runner)->data);
        assert(compare_result);
        runner = compare_result > 0 ? &((*runner)->right) : &((*runner)->left);
    }

    node->parent = runner_prev;
    *runner = node;
    
    return NodeToIter(node);
}

static int IsLeftChild(bst_node_t* node)
{
    return node->parent->left == node;
}

static bst_node_t* RunLeft(bst_node_t* node)
{
    while(node->left != NULL)
    {
        node = node->left;
    }

    return node;
}

static bst_node_t* RunRight(bst_node_t* node)
{
    while(node->right != NULL)
    {
        node = node->right;
    }

    return node;
}

static bst_iter_t RemoveLeaf(bst_node_t* node)
{
    bst_node_t* next = node->parent;

    if(IsLeftChild(node))
    {
        next->left = NULL;
    }

    else
    {
        next->right = NULL;
    }

    free(node);

    return NodeToIter(next);
}

static bst_iter_t RemoveChild(bst_node_t* node, bst_node_t* next)
{
    bst_node_t* parent = node->parent;

    next->parent = parent;

    if(IsLeftChild(node))
    {
        parent->left = next;
    }

    else
    {
        parent->right = next;
    }

    free(node);

    return NodeToIter(next);
}

static bst_iter_t RemoveLeftChild(bst_node_t* node)
{
    bst_node_t* next = node->left;
    
    return RemoveChild(node, next);
}

static bst_iter_t RemoveRightChild(bst_node_t* node)
{
    bst_node_t* next = node->right;
    
    return RemoveChild(node, next);
}

static bst_iter_t RemoveNode(bst_node_t* node)
{
    bst_node_t* next = NULL;

    if(node->left == NULL && node->right == NULL)
    {
        return RemoveLeaf(node);
    }

    else if(node->right == NULL)
    {
        return RemoveLeftChild(node);
    }

    else if(node->left == NULL)
    {
        return RemoveRightChild(node);
    }

    next = IterToNode(BSTNext(NodeToIter(node)));
    node->data = next->data;

    if(next->right != NULL)
    {
        return RemoveRightChild(next);
    }

    return RemoveLeaf(next);
}

/*****************************************************************************/

bst_t* BSTCreate(compare_func_t compare_func)
{
    bst_t* bst = NULL;
    bst_node_t dummy = {0};

    assert(compare_func);

    bst = (bst_t*)malloc(sizeof(bst_t));

    if(bst == NULL)
    {
        return NULL;
    }

    bst->compare_func = compare_func;
    bst->dummy = dummy;

    return bst;
}

void BSTDestroy(bst_t* bst)
{
    bst_iter_t iter;

    assert(bst);

    iter = BSTBegin(bst);

    while(!BSTIsEmpty(bst))
    {
        iter = RemoveNode(IterToNode(iter));
    }

    free(bst);
}

size_t BSTGetSize(const bst_t* bst)
{
    bst_iter_t iter;
    size_t count = 0;

    assert(bst);

    iter = BSTBegin(bst);

    while(!BSTIsSameIter(iter, BSTEnd(bst)))
    {
        ++count;
        iter = BSTNext(iter);
    }

    return count;
}

int BSTIsEmpty(const bst_t* bst)
{
    assert(bst);

    return bst->dummy.left == NULL;
}

bst_iter_t BSTInsert(bst_t* bst, const void* data)
{
    bst_node_t* node = NULL;

    assert(bst);

    node = (bst_node_t*)malloc(sizeof(bst_node_t));

    if(node == NULL)
    {
        return BSTEnd(bst);
    }

    node->data = (void*)data;
    node->parent = NULL;
    node->right = NULL;
    node->left = NULL;
    
    return PlaceNode(bst, node);
}

void* BSTRemove(bst_iter_t iter)
{
    bst_node_t* node = IterToNode(iter);
    void* data = NULL;

    assert(node);

    data = node->data;
    RemoveNode(node);
    
    return data;
}

int BSTIsSameIter(bst_iter_t one, bst_iter_t other)
{
    return one.node == other.node;
}

void* BSTGetValue(bst_iter_t iter)
{
    return iter.node->data;
}

bst_iter_t BSTBegin(const bst_t* bst)
{
    bst_node_t* node = NULL;

    assert(bst);

    node = RunLeft((bst_node_t*)&(bst->dummy));

    return NodeToIter(node);   
}

bst_iter_t BSTEnd(const bst_t* bst)
{
    assert(bst);

    return NodeToIter((bst_node_t*)&(bst->dummy));
}

bst_iter_t BSTNext(bst_iter_t iter)
{
    bst_node_t* node = IterToNode(iter);
 
    if(node->right != NULL)
    {
        node = RunLeft(node->right);

        return NodeToIter(node);
    }
    
    while(!IsLeftChild(node))
    {
        node = node->parent;
    }

    return NodeToIter(node->parent);
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
    bst_node_t* node = IterToNode(iter);

    if(node->left != NULL)
    {
        node = RunRight(node->left);

        return NodeToIter(node);
    }
    
    while(IsLeftChild(node))
    {
        node = node->parent;
    }

    return NodeToIter(node->parent);
}

bst_iter_t BSTFind(const bst_t* bst, const void* data)
{
    bst_node_t* iter = NULL;
    int compare_result = 0;

    assert(bst);

    iter = bst->dummy.left;

    while(iter != NULL)
    {
        compare_result = bst->compare_func(data, iter->data);

        if(compare_result > 0)
        {
            iter = iter->right;
        }

        else if(compare_result < 0)
        {
            iter = iter->left;
        }

        else
        {
            return NodeToIter(iter);
        }
    }

    return BSTEnd(bst);
}

int BSTForEach(bst_iter_t from, bst_iter_t to, action_func_t action_func,
                                                                void* param)
{
    int action_result = 0;

    assert(action_func);

    while(!BSTIsSameIter(from, to))
    {
        action_result = action_func(IterToNode(from)->data, param);
        
        if(action_result != 0)
        {
            return action_result;
        }

        from = BSTNext(from);
    }

    return 0;
}