#include <string.h>

#include "test_macros.h"
#include "ilrd_uid.h"

#include "avl.h"

static ilrd_uid_t uids[128];
static size_t multi_find[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
static size_t pre_order[128];
static size_t post_order[128];
static size_t helper_counter = 0;

static int CompareFunc(const void* data1, const void* data2)
{
	const ilrd_uid_t* uid1 = (const ilrd_uid_t*)data1;
	const ilrd_uid_t* uid2 = (const ilrd_uid_t*)data2;
	int memcmp_res;
	
	if (UIDIsSame(*uid1, *uid2))
	{
		return 0;
	}
	
	if (UIDIsSame(*uid1, bad_uid))
	{
		return 1;
	}
	
	if (UIDIsSame(*uid2, bad_uid))
	{
		return -1;
	}
	
	memcmp_res = memcmp(uid1->ip, uid2->ip, sizeof(uid1->ip));
	
	if (memcmp_res != 0)
	{
		return memcmp_res;
	}
	
	if (uid1->pid != uid2->pid)
	{
		return uid1->pid - uid2->pid;
	}
	
	if (uid1->time != uid2->time)
	{
		return uid1->time - uid2->time;
	}
	
	return uid1->counter - uid2->counter;
}

static int Foreach1(void* data, void* param)
{
	int* temp = (int*)param;
	(void)data;
	++(*temp);
	if(*temp > 29)
	{
		return *temp;
	}
	return 0;
}

static int IsMatching(const void* data, const void* param)
{
	size_t* arr = (size_t*)param;
	ilrd_uid_t* uid = (ilrd_uid_t*)data;
	size_t i = 0;

	for (; i < 10; ++i)
	{
		if (UIDIsSame(*uid, uids[arr[i]]))
		{
			return 1;
		}
	}

	return 0;
}

static void BuildPreTable(size_t size, size_t offset)
{
	pre_order[helper_counter] = offset + size / 2 - 1;
	++helper_counter;
	if (size / 2  > 1)
	{
		BuildPreTable(size / 2, offset);
		BuildPreTable(size / 2, size / 2  + offset);
	}
}



static void BuildBalancedTree(avl_t* avl, size_t size, size_t offset)
{
	AVLInsert(avl, uids + offset + size / 2 - 1);
	if (size / 2 > 1)
	{
		BuildBalancedTree(avl, size / 2, offset);
		BuildBalancedTree(avl, size / 2, size / 2 + offset);
	}
}

static void BuildPostTable(size_t size, size_t offset)
{
	if (size / 2 > 1)
	{
		BuildPostTable(size / 2, offset);
		BuildPostTable(size / 2, size / 2 + offset);
	}
	post_order[helper_counter] = offset + size / 2 - 1;
	++helper_counter;
}


static void BuildPostOrder(avl_t* avl, size_t size, size_t offset)
{
	if (size / 2 > 1)
	{
		BuildPostOrder(avl, size / 2, offset);
		BuildPostOrder(avl, size / 2, size / 2 + offset);
	}
	AVLInsert(avl, uids + offset + size / 2 - 1);
}

static void CommonTest1(avl_t* avl)
{
	size_t i = 0;

	TEST("Height", AVLGetHeight(avl), 7);
	TEST("Size", AVLGetSize(avl), 128);

	for (i = 0; i < 128; ++i)
	{
		TEST("Find after insert", AVLFind(avl, uids + i) != NULL, 1);
	}

	for (i = 0; i < 128; i += 2)
	{
		AVLRemove(avl, uids + i);
	}

	TEST("Height", AVLGetHeight(avl), 6);
	TEST("Size", AVLGetSize(avl), 64);

	for (i = 0; i < 128; ++i)
	{
		TEST("Find after remove", AVLFind(avl, uids + i) != NULL, i % 2);
	}

	for (i = 0; i < 128; ++i)
	{
		AVLRemove(avl, uids + i);
	}

	TEST("Height", AVLGetHeight(avl), 0);
	TEST("Size", AVLGetSize(avl), 0);
	TEST("Is Empty", AVLIsEmpty(avl), 1);
}

static void CommonTest2(avl_t* avl)
{
	size_t i = 0;
	size_t counter = 0;
	ssize_t num = 0;
	int val;
	dl_list_t* list = DLListCreate();
	dll_iter_t iter;

	TEST("Height", AVLGetHeight(avl), 7);
	TEST("Size", AVLGetSize(avl), 128);
	TEST("Is Empty", AVLIsEmpty(avl), 0);

	counter = 0;
	val = AVLForEach(avl, Foreach1, &counter, IN);
	TEST("val is 30", val, 30);
	TEST("counter is 30", counter, 30);
	counter = 0;
	val = AVLForEach(avl, Foreach1, &counter, PRE);
	TEST("val is 30", val, 30);
	TEST("counter is 30", counter, 30);
	counter = 0;
	val = AVLForEach(avl, Foreach1, &counter, POST);
	TEST("val is 30", val, 30);
	TEST("counter is 30", counter, 30);

/*********ADVANCED SECTION****************/

	num =  AVLMultipleFind(avl, IsMatching, multi_find, list);
	TEST("Num is 10", num, 10);
	iter = DLListBegin(list);

	for (i = 0; i < 10; ++i)
	{
		TEST("List contains the values", DLListGetValue(iter), uids + multi_find[i]);
		iter = DLListNext(iter);
	}

	DLListDestroy(list);
	list = DLListCreate();

	num =  AVLMultipleRemove(avl, IsMatching, multi_find, list);
	TEST("Num is 10", num, 10);
	iter = DLListBegin(list);

	for (i = 0; i < 10; ++i)
	{
		TEST("List contains the values", DLListGetValue(iter), uids + multi_find[i]);
		iter = DLListNext(iter);
	}

	num = AVLMultipleFind(avl, IsMatching, multi_find, list);
	TEST("No more of these elements", num, 0);
	DLListDestroy(list);
}

static void TestAVLPart1(avl_t* avl)
{
	size_t i = 0;
	CommonTest1(avl);

	for (i = 0; i < 128; ++i)
	{
		AVLInsert(avl, uids + i);
	}

	CommonTest2(avl);
}

static void TestAVLPart2(avl_t* avl)
{
	size_t i = 0;
	CommonTest1(avl);

	for (i = 128; i > 0; --i)
	{
		AVLInsert(avl, uids + i - 1);
	}

	CommonTest2(avl);
}

static void TestAVLPart3(avl_t* avl)
{
	CommonTest1(avl);
	BuildBalancedTree(avl, 128, 0);
	AVLInsert(avl, uids + 127);
	CommonTest2(avl);
}

static void TestAVLPart4(avl_t* avl)
{
	size_t i = 0;
	size_t counter = 0;
	ssize_t num = 0;
	int val;
	dl_list_t* list = DLListCreate();
	dll_iter_t iter;

	TEST("Height", AVLGetHeight(avl), 8);
	TEST("Size", AVLGetSize(avl), 128);

	for (i = 0; i < 128; ++i)
	{
		TEST("Find after insert", AVLFind(avl, uids + i) != NULL, 1);
	}

	for (i = 0; i < 128; i += 2)
	{
		AVLRemove(avl, uids + i);
	}

	TEST("Height", AVLGetHeight(avl), 6);
	TEST("Size", AVLGetSize(avl), 64);

	for (i = 0; i < 128; ++i)
	{
		TEST("Find after remove", AVLFind(avl, uids + i) != NULL, i % 2);
	}

	for (i = 0; i < 128; ++i)
	{
		AVLRemove(avl, uids + i);
	}

	TEST("Height", AVLGetHeight(avl), 0);
	TEST("Size", AVLGetSize(avl), 0);
	TEST("Is Empty", AVLIsEmpty(avl), 1);
	BuildPostOrder(avl, 128, 0);
	AVLInsert(avl, uids + 127);

	TEST("Height", AVLGetHeight(avl), 8);
	TEST("Size", AVLGetSize(avl), 128);
	TEST("Is Empty", AVLIsEmpty(avl), 0);

	counter = 0;
	val = AVLForEach(avl, Foreach1, &counter, IN);
	TEST("val is 30", val, 30);
	TEST("counter is 30", counter, 30);
	counter = 0;
	val = AVLForEach(avl, Foreach1, &counter, PRE);
	TEST("val is 30", val, 30);
	TEST("counter is 30", counter, 30);
	counter = 0;
	val = AVLForEach(avl, Foreach1, &counter, POST);
	TEST("val is 30", val, 30);
	TEST("counter is 30", counter, 30);

/*********ADVANCED SECTION****************/

	num =  AVLMultipleFind(avl, IsMatching, multi_find, list);
	TEST("Num is 10", num, 10);
	iter = DLListBegin(list);

	for (i = 0; i < 10; ++i)
	{
		TEST("List contains the values", DLListGetValue(iter), uids + multi_find[i]);
		iter = DLListNext(iter);
	}

	DLListDestroy(list);
	list = DLListCreate();

	num =  AVLMultipleRemove(avl, IsMatching, multi_find, list);
	TEST("Num is 10", num, 10);
	iter = DLListBegin(list);

	for (i = 0; i < 10; ++i)
	{
		TEST("List contains the values", DLListGetValue(iter), uids + multi_find[i]);
		iter = DLListNext(iter);
	}

	num = AVLMultipleFind(avl, IsMatching, multi_find, list);
	TEST("No more of these elements", num, 0);
	DLListDestroy(list);
}


static void TestAVL(void)
{
	avl_t* avl = AVLCreate(CompareFunc);
	size_t i = 0;

	for (; i < 128; ++i)
	{
		uids[i] = UIDCreate();
	}

	helper_counter = 0;
	BuildPreTable(128, 0);
	pre_order[127] = 127;
	helper_counter = 0;
	BuildPostTable(128, 0);
	post_order[120] = 127;
	post_order[121] = 126;
	post_order[122] = 125;
	post_order[123] = 123;
	post_order[124] = 119;
	post_order[125] = 111;
	post_order[126] = 95;
	post_order[127] = 63;

	TEST("Check insanity", AVLIsEmpty(avl), 1);
	TEST("Check insanity", AVLGetSize(avl), 0);

	for (i = 0; i < 128; ++i)
	{
		AVLInsert(avl, uids + i);
	}

	TestAVLPart1(avl);
	AVLDestroy(avl);

	avl = AVLCreate(CompareFunc);

	for (i = 128; i > 0; --i)
	{
		AVLInsert(avl, uids + i - 1);
	}

	TestAVLPart2(avl);
	AVLDestroy(avl);

	avl = AVLCreate(CompareFunc);
	BuildBalancedTree(avl, 128, 0);
	AVLInsert(avl, uids + 127);
	TestAVLPart3(avl);
	AVLDestroy(avl);

	avl = AVLCreate(CompareFunc);
	BuildPostOrder(avl, 128, 0);
	AVLInsert(avl, uids + 127);
	TestAVLPart4(avl);
	AVLDestroy(avl);
}

int main(void)
{
	TestAVL();	
	PASS;
	return 0;
}