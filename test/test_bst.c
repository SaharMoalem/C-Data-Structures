#include "bst.h"
#include "test_macros.h"

#define ARR_SIZE 8

static int CompareFunc(const void* one, const void* other)
{
    return ((*(int*)one) - (*(int*)other));
}

static int ForEachFunc(void* data, void* res)
{
    ++(*(int*)res);
    return 0;
}

void TestBST(void)
{
    bst_t* bst = BSTCreate(CompareFunc);
    bst_iter_t iter = {0};
    size_t i = 0;
    int res = 0;
    int arr[ARR_SIZE] = {11, 4 , 8, 15, 3, 9, 58, 14};

    TEST("BST is empty test", BSTIsEmpty(bst), 1);
    for (i = 0; i < ARR_SIZE; ++i)
    {
        iter = BSTInsert(bst, &arr[i]);
        TEST("Size test", BSTGetSize(bst), i + 1);
    }

    iter = BSTBegin(bst);
    TEST("First value is 3 test", *(int*)BSTGetValue(iter), arr[4]);

    iter = BSTFind(bst, &arr[3]);
    TEST("Found result in bst test", BSTIsSameIter(iter, BSTEnd(bst)), 0);
    TEST("Found 15 in BST", *(int*)BSTGetValue(iter), arr[3]);

    iter = BSTNext(iter);
    TEST("58 comes after 15 in BST", *(int*)BSTGetValue(iter), arr[6]);

    iter = BSTFind(bst, &arr[ARR_SIZE - 1]);
    TEST("Found result in bst test", BSTIsSameIter(iter, BSTEnd(bst)), 0);
    TEST("Found 14 in BST", *(int*)BSTGetValue(iter), arr[ARR_SIZE - 1]);
    iter = BSTPrev(iter);
    TEST("11 comes before 14 in BST", *(int*)BSTGetValue(iter), arr[0]);

    res = *(int*)BSTRemove(iter);
    TEST("Removed 11 from bst test", res, arr[0]);
    TEST("Size test", BSTGetSize(bst), ARR_SIZE - 1);
    TEST("BST is empty test", BSTIsEmpty(bst), 0);

    res = 0;
    BSTForEach(BSTBegin(bst), BSTEnd(bst), ForEachFunc, &res);
    TEST("ForEach ran for 7 times", res, 7);

    BSTDestroy(bst);
}

int main(void)
{
	TestBST();
	PASS;
	return 0;
}
