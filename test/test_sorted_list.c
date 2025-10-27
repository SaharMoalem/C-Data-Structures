#include <string.h>

#include "test_macros.h"
#include "test_structs.h"

#include "sorted_list.h"

static int Compare(const void* param1, const void* param2)
{
    const student_t* student1 = (const student_t*)param1;
    const student_t* student2 = (const student_t*)param2;
    return student1->grades.real.math - student2->grades.real.math;
}

static int Match(const void* data, const void* param)
{
    const student_t* student = (const student_t*)data;
    const char* name = (const char*)param;
    return 0 == strcmp(student->first_name, name); 
}

static int ForEach(void* data, void* param)
{
    student_t* student = (student_t*)data;
    student->grades.real.physics = (unsigned char)*(int*)param;
    return -5;
}

static void TestSortedList(void)
{
    sorted_list_t* list1 = CreateSortedList(Compare);
    sorted_list_t* list2 = CreateSortedList(Compare);
    sorted_list_iter_t iter;
    student_t stud;
    size_t i = 0;
    int result_value = 105;
	
    for(; i < ARRAY_SIZE; ++i)
    {
        if (i & 0x01)
	{
	    SortedListInsert(list1, students_array + i); 
      	}
	else
	{
	    SortedListInsert(list2, students_array + i);
	}
    }
	
    TEST("After insert", SortedListGetValue(SortedListBegin(list1)), students_array + 3);
    TEST("After insert", SortedListGetValue(SortedListBegin(list2)), students_array + 4);
    TEST("After insert", SortedListSize(list1), 2);
    TEST("After insert", SortedListSize(list2), 3);
    result_value = SortedListForEach(SortedListBegin(list2), SortedListEnd(list2), ForEach, &result_value);
    TEST("After ForEach", result_value, -5);
    result_value = ((const student_t*)SortedListGetValue(SortedListPrev(SortedListEnd(list2))))->grades.real.physics; 
    TEST("After ForEach", result_value, 95);
    iter = SortedListFindIf(SortedListBegin(list1), SortedListEnd(list1), Match, "Amit");
    TEST("Find if", SortedListIsSameIter(iter, SortedListEnd(list1)), 1);
    stud.grades.real.math = 90;
    iter = SortedListFind(list2, SortedListBegin(list2), SortedListEnd(list2), &stud);
    TEST("Find", SortedListIsSameIter(iter, SortedListEnd(list2)), 0);
    SortedListMerge(list2, list1);
    TEST("After Merge", SortedListGetValue(SortedListPrev(SortedListEnd(list2))), students_array + 1);
    TEST("After Merge", SortedListIsEmpty(list1), 1);
	
    for(; i < ARRAY_SIZE; ++i)
    {
        if (i & 0x01)
	{
	    SortedListInsert(list2, students_array + i); 
	}
	else
	{
	    SortedListInsert(list1, students_array + i);
	}
    }
	
    SortedListDestroy(list1);
    SortedListDestroy(list2);
}


int main(void)
{
    TestSortedList();
    PASS;
    return 0;
}
