#include "heap.h"
#include "test_macros.h"

#define ARR_SIZE 10

static int CompareFunc(const void* data, const void* param)
{
    return *(int*)data - *(int*)param;
}

static int MatchFunc(const void* data, const void* param)
{
    return *(int*)data == *(int*)param;
}

void TestHeap(void)
{
    heap_t* heap = HeapCreate(CompareFunc);
    int arr[ARR_SIZE] = {11, 58, 12, 55, 48, 1, 14, 99, 87, 3};
    int* data = NULL;
    size_t i = 0;

    for (i = 0; i < ARR_SIZE; ++i)
    {
        HeapPush(heap, &arr[i]);
        TEST("Insert success test",HeapSize(heap), i + 1);
    }

    TEST("Is empty test",HeapIsEmpty(heap), 0);
    data = HeapPeek(heap);
    HeapPop(heap);
    TEST("1 is min value",*data, arr[5]);
    data = HeapPeek(heap);
    HeapPop(heap);
    TEST("3 is min value",*data, arr[ARR_SIZE - 1]);
    data = HeapRemove(heap, arr + 3, MatchFunc);
    TEST("55 removed", *data, arr[3]);
    TEST("Size of heap", HeapSize(heap), 7);

    HeapDestroy(heap);
}

int main(void)
{
	TestHeap();
	PASS;
	return 0;
}
