#include <assert.h>      /*assert*/
#include <stdlib.h>      /*malloc, free*/

#include "avl.h"

#define MAX(a,b) (a > b ? a : b)

typedef struct avl_node {
    void* data;
    struct avl_node* left;
    struct avl_node* right;
    size_t height;
} avl_node_t;

struct avl {
    compare_func_t compare_func;
    avl_node_t* root;
};

avl_t* AVLCreate(compare_func_t compare_func)
{
    avl_t* avl = NULL;

    assert(compare_func);

    avl = (avl_t*)malloc(sizeof(avl_t));

    if(!avl)
    {
        return NULL;
    }

    avl->compare_func = compare_func;
    avl->root = NULL;

    return avl;
}

static void DestroyNodes(avl_node_t* node)
{
    if(!node)
    {
        return;
    }

    DestroyNodes(node->left);
    DestroyNodes(node->right);
    free(node);
}

void AVLDestroy(avl_t* avl)
{
    assert(avl);

    DestroyNodes(avl->root);
    free(avl);
}

static void UpdateHeight(avl_node_t* node)
{
    size_t left_height = node->left ? node->left->height : 0;
    size_t right_height = node->right ? node->right->height : 0;

    node->height = (node->left || node->right) + MAX(left_height, right_height);
}

static avl_node_t* RotateLeft(avl_node_t* root)
{
    avl_node_t* pivot = root->right;
    avl_node_t* pivot_left = pivot->left;

    pivot->left = root;
    root->right = pivot_left;    
    UpdateHeight(root);
    UpdateHeight(pivot);

    return pivot;
}

static avl_node_t* RotateRight(avl_node_t* root)
{
    avl_node_t* pivot = root->left;
    avl_node_t* pivot_right = pivot->right;

    pivot->right = root;
    root->left = pivot_right;   
    UpdateHeight(root);
    UpdateHeight(pivot);

    return pivot;
}

static int GetBalanceFactor(avl_node_t* node)
{
    int left_height = node->left ? (int)node->left->height : 0;
    int right_height = node->right ? (int)node->right->height : 0;

    return right_height - left_height - (!node->right) + (!node->left);
}

static avl_node_t* CheckBalance(avl_node_t* root)
{ 
    int balance_factor = GetBalanceFactor(root);
    int child_balance_factor = 0;

    if(balance_factor > 1)
    {
        child_balance_factor = GetBalanceFactor(root->right);

        if(child_balance_factor < 0)
        {
            root->right = RotateRight(root->right);
        }

        root = RotateLeft(root);
    }

    else if(balance_factor < -1)
    {
        child_balance_factor = GetBalanceFactor(root->left);
        if(child_balance_factor > 0)
        {
            root->left = RotateLeft(root->left);
        }

        root = RotateRight(root);
    }

    return root;
}

static avl_node_t* PlaceNode(avl_node_t* root, compare_func_t compare_func,
                                                        avl_node_t* new_node)
{
    int compare_result = 0;

    if(!root)
    {
        return new_node;
    }

    compare_result = compare_func(root->data, new_node->data);

    assert(compare_result);

    if(compare_result > 0)
    {
        root->left = PlaceNode(root->left, compare_func, new_node);
    }

    else
    {
        root->right = PlaceNode(root->right, compare_func, new_node);
    }

    UpdateHeight(root);
    return CheckBalance(root);
}

int AVLInsert(avl_t* avl, const void* data)
{
    avl_node_t* node = NULL;

    assert(avl);

    node = (avl_node_t*)malloc(sizeof(avl_node_t));

    if(!node)
    {
        return -1;
    }

    node->data = (void*)data;
    node->left = NULL;
    node->right = NULL;
    node->height = 0;
    avl->root = PlaceNode(avl->root, avl->compare_func, node);

    return 0;
}

static avl_node_t* RemoveNextInOrder(avl_node_t* node_to_cpy, avl_node_t* node)
{
    avl_node_t* node_cpy = NULL;

    if(node->left)
    {
        node->left = RemoveNextInOrder(node_to_cpy, node->left);
        UpdateHeight(node);
        return CheckBalance(node);
    }

    node_cpy = node->right;
    node_to_cpy->data = node->data;
    free(node);

    return node_cpy;
}

static avl_node_t* RemoveNode(avl_node_t* node)
{
    avl_node_t* child = NULL;

    if(node->right)
    {
        node->right = RemoveNextInOrder(node, node->right);
        UpdateHeight(node);
        return CheckBalance(node);
    }

    child = node->left;
    free(node);
    
    return child; 
}

static avl_node_t* FindNodeToRemove(avl_node_t* root,
                                compare_func_t compare_func, const void* param)
{
    int compare_result = 0;

    if(!root)
    {
        return NULL;
    }

    compare_result = compare_func(root->data, param);

    if(compare_result == 0)
    {
        root = RemoveNode(root);
        return root;
    }

    if(compare_result > 0)
    {
        root->left = FindNodeToRemove(root->left, compare_func, param);
    }

    else
    {
        root->right = FindNodeToRemove(root->right, compare_func, param);
    }

    UpdateHeight(root);

    return CheckBalance(root);
}

void AVLRemove(avl_t* avl, const void* param)
{
    assert(avl);

    avl->root = FindNodeToRemove(avl->root, avl->compare_func, param);
}

int AVLIsEmpty(const avl_t* avl)
{
    assert(avl);

    return avl->root == NULL;
}

size_t AVLGetHeight(const avl_t* avl)
{
    assert(avl);

    return avl->root ? avl->root->height : 0;
}

static size_t CountNodes(avl_node_t* node)
{
    if(!node)
    {
        return 0;
    }

    return 1 + CountNodes(node->left) + CountNodes(node->right);
}

size_t AVLGetSize(const avl_t* avl)
{
    assert(avl);

    return CountNodes(avl->root);
}

static void* FindNode(avl_node_t* node, compare_func_t compare_func,
                                                            const void* param)
{
    int compare = 0;

    if(!node)
    {
        return NULL;
    }

    compare = compare_func(node->data, param);

    if(compare == 0)
    {
        return node->data;
    }

    if(compare > 0)
    {
        return FindNode(node->left, compare_func, param);
    }

    return FindNode(node->right, compare_func, param);
}

void* AVLFind(const avl_t* avl, const void* param)
{
    assert(avl);

    return FindNode(avl->root, avl->compare_func, param);
}

static int PreOrderForEach(avl_node_t* node, action_func_t action_func,
                                                                    void* param)
{
    int action_result = 0;

    if(!node)
    {
        return 0;
    }

    action_result = action_func(node->data, param);

    if(action_result != 0)
    {
        return action_result;
    }

    action_result = PreOrderForEach(node->left, action_func, param);

    if(action_result != 0)
    {
        return action_result;
    }

    return PreOrderForEach(node->right, action_func, param);
}

static int InOrderForEach(avl_node_t* node, action_func_t action_func,
                                                                    void* param)
{
    int action_result = 0;

    if(!node)
    {
        return 0;
    }

    action_result = InOrderForEach(node->left, action_func, param);

    if(action_result != 0)
    {
        return action_result;
    }

    action_result = action_func(node->data, param);

    if(action_result != 0)
    {
        return action_result;
    }

    return InOrderForEach(node->right, action_func, param);
}

static int PostOrderForEach(avl_node_t* node, action_func_t action_func,
                                                                    void* param)
{
    int action_result = 0;

    if(!node)
    {
        return 0;
    }

    action_result = PostOrderForEach(node->left, action_func, param);

    if(action_result != 0)
    {
        return action_result;
    }

    action_result = PostOrderForEach(node->right, action_func, param);

    if(action_result != 0)
    {
        return action_result;
    }

    return action_func(node->data, param);
}

int AVLForEach(const avl_t* avl, action_func_t action_func, void* param,
                                                        traversal_order_t order)
{
    assert(avl);
    assert(action_func);

    switch(order)
    {
        case PRE:
            return PreOrderForEach(avl->root, action_func, param);
        
        case IN:
            return InOrderForEach(avl->root, action_func, param);

        default:
            return PostOrderForEach(avl->root, action_func, param);         
    } 
}

static size_t TraverseAndPushToList(avl_t* avl, avl_node_t* node,
        is_match_t match_func, void* param, dl_list_t* out_list, int to_remove)
{
    int is_match = 0;
    size_t left_matches = 0;
    dll_iter_t iter = DLListEnd(out_list);

    if(!node)
    {
        return 0;
    }
    
    left_matches = TraverseAndPushToList(avl, node->left, match_func, param,
                                                        out_list, to_remove);  
    is_match = match_func(node->data, param);

    if(is_match)
    {
        iter = DLListPushBack(out_list, node->data);
        
        if(to_remove && !DLListIsSameIter(iter, DLListEnd(out_list)))
        {
            AVLRemove(avl, node->data);
        }
    }
    
    return (size_t)is_match + left_matches + TraverseAndPushToList(avl,
                        node->right, match_func, param, out_list, to_remove);
}

static ssize_t FindAndMaybeRemove(const avl_t* avl, is_match_t match_func,
                                void* param, dl_list_t* out_list, int to_remove)
{
    size_t curr_list_size = DLListSize(out_list);
    size_t result = TraverseAndPushToList((avl_t*)avl, avl->root, match_func,
                                                    param, out_list, to_remove);

    return curr_list_size + result == DLListSize(out_list) ? (ssize_t)result :
                                                                            -1;
}

ssize_t AVLMultipleFind(const avl_t* avl, is_match_t match_func, void* param,
                                                            dl_list_t* out_list)
{
    assert(avl);
    assert(match_func);
    assert(out_list);

    return FindAndMaybeRemove(avl, match_func, param, out_list, 0);
}

ssize_t AVLMultipleRemove(const avl_t* avl, is_match_t match_func, void* param,
                                                            dl_list_t* out_list)
{
    assert(avl);
    assert(match_func);
    assert(out_list);

    return FindAndMaybeRemove(avl, match_func, param, out_list, 1);
}