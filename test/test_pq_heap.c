#include <string.h> /*strcmp*/

#include "pq_heap.h"
#include "test_macros.h"
#include "test_structs.h"

int FindStudent (const void* data, const void* element)
{
    student_t* one = (student_t*) data;
    student_t* other = (student_t*) element;
	
    return (!strcmp(one->first_name, other->first_name) &&
            !strcmp(one->last_name, other->last_name));
}

int CompareStudents(const void* cmp, const void* to)
{
    student_t* one = (student_t*) cmp;
    student_t* other = (student_t*) to;
	
    return ((one->grades.real.math) - (other->grades.real.math));
}


void TestPriorityQueue(void)
{
    size_t i = 0;
    student_t* student = students_array;
    pq_t* pq = PQCreate(CompareStudents);

    TEST("Check PQIsEmpty", PQIsEmpty(pq), 1);
			
    for(i = 0; i < ARRAY_SIZE; ++i)
    {
    	PQEnqueue(pq, &students_array[i]);
    }
    TEST("Check PQIsEmpty again", PQIsEmpty(pq), 0);
    student = (student_t*)PQPeek(pq);
    TEST("Check PQPeek", student, &students_array[4]);
    student = (student_t*)PQErase(pq, FindStudent, &students_array[3]);
    TEST("Check PQErase", student, &students_array[3]);
    student = (student_t*)PQErase(pq, FindStudent, &students_array[2]);
    TEST("Check PQErase again", student, &students_array[2]);
    PQDequeue(pq);
    TEST("Check PQSize", PQSize(pq), i - 3);
    PQClear(pq);
    TEST("Check PQClear", PQIsEmpty(pq), 1);
        
    PQDestroy(pq);
}

int main(void)
{
    TestPriorityQueue();
    PASS;
    return 0;
}