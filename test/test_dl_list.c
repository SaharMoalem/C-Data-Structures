#include "test_macros.h" 
#include "test_structs.h" 

#include "dl_list.h"

static int FindFunc(const void* data, const void* param)
{
    student_t* stud = (student_t*)data;
    student_t* cmp = (student_t*)param;
    return cmp->grades.real.math < stud->grades.real.math || cmp->grades.real.physics < stud->grades.real.physics || cmp->grades.real.computers < stud->grades.real.computers;
}

void TestDLList(void)
{
    dl_list_t* list = DLListCreate();
    dl_list_t* list2 = DLListCreate();
    size_t i = 0;
    student_t stud;
    for(; i < ARRAY_SIZE; ++i)
    {
    	DLListPushBack(list, students_array + i);
    }
	
    TEST("After push back", DLListSize(list), ARRAY_SIZE);
    TEST("After push back value", DLListGetValue(DLListNext(DLListBegin(list))), students_array + 1);
    TEST("After push back value", DLListGetValue(DLListPrev(DLListEnd(list))), students_array + ARRAY_SIZE - 1);
	
    for(i = ARRAY_SIZE; i > 0; --i)
    {
    	DLListPushFront(list, students_array + i - 1);
    }
	
    TEST("After push front", DLListSize(list), 2 * ARRAY_SIZE);
    TEST("After push front value", DLListGetValue(DLListNext(DLListBegin(list))), students_array + 1);
    TEST("After push front value", DLListGetValue(DLListPrev(DLListEnd(list))), students_array + ARRAY_SIZE - 1);
	
    DLListPopFront(list);
    DLListPopBack(list);
	
    TEST("After pop front value", DLListGetValue(DLListNext(DLListBegin(list))), students_array + 2);
    TEST("After pop back value", DLListGetValue(DLListPrev(DLListEnd(list))), students_array + ARRAY_SIZE - 2);
    TEST("Is Empty", DLListIsEmpty(list), 0);
	
    stud.grades.real.math = 90;
    stud.grades.real.physics = 90;
    stud.grades.real.computers = 90;
	
    dll_iter_t goal = DLListBegin(list);
    dll_iter_t find = DLListFind(DLListBegin(list), DLListEnd(list), FindFunc, &stud);
	
    TEST("Find success", DLListIsSameIter(goal, find), 1);
   
    DLListMultiFind(DLListBegin(list), DLListEnd(list), FindFunc, &stud, list2);
	
    TEST("Multi find works", DLListSize(list2), 3);
	
    DLListSplice(DLListBegin(list2), DLListEnd(list2), DLListNext(DLListNext(DLListBegin(list))));
	
    TEST("Splice two lists", DLListSize(list2), 0);
    TEST("Splice two lists", DLListSize(list), ARRAY_SIZE * 2 + 1);
	
    while(DLListSize(list) > 0)
    {
    	DLListPushFront(list, DLListPopFront(list));
    	DLListPushFront(list2, DLListPopFront(list));
    }
	
    DLListDestroy(list);
    DLListDestroy(list2);
}

int main(void)
{
    TestDLList();
    PASS;
	return 0;
}
