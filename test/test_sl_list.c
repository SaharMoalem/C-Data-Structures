#include <stdio.h>                 

#include "test_macros.h"
#include "test_structs.h"
#include "sl_list.h"

static int CustomFind(const void* one, const void* other)
{
    int temp = one == other;
    return temp;
}

static int SetSportsGrade(void* student, void* grade)
{
    ((student_t*)student)->grades.sports = *((float*)grade);
    return 0;
}

static void TestSingleLinkedList(void)
{
    sl_list_t* list = SLListCreate();
    iterator_t iter;
    float grade = 100;
    size_t i = 0;
    
    TEST("Check SLListIsEmpty", SLListIsEmpty(list), 1);
    for( ; i < ARRAY_SIZE; ++i)
    {
    	SLListInsertBefore(SLListEnd(list), students_array + i);
    }
	
    TEST("Check SLListCount", SLListCount(list), ARRAY_SIZE);
    TEST("Check SLListGetValue", SLListGetValue(SLListNext(SLListBegin(list))), students_array + 1);
    iter = SLListNext(SLListNext(SLListBegin(list)));
    SLListSetValue(iter, students_array);
    SLListRemoveElement(iter);
    TEST("Check SLListRemoveElement", SLListGetValue(iter), students_array + 3);
    TEST("Check SLListFind", SLListIsSameIter(SLListFind(iter, SLListEnd(list), CustomFind, students_array), SLListEnd(list)), 1);
    SLListForEach(iter, SLListNext(iter), SetSportsGrade, &grade);
    TEST("Check SLListForEach", ((student_t*)SLListGetValue(iter))->grades.sports, grade);
    SLListDestroy(list);
}

int main(void)
{
    TestSingleLinkedList();
    PASS;
    return 0;
}
