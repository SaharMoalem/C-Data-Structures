#include <assert.h> /* assert */
#include <string.h> /* strcmp */

#include "../include/stack.h" /* function decleration */

#define ARRAY_SIZE (5)

typedef struct humanistic_grade
{
	unsigned char literature;
	unsigned char history;
	unsigned char language;
} humanistic_grade_t;

typedef struct real_grade
{
	unsigned char math;
	unsigned char physics;
	unsigned char computers;
} real_grade_t;

typedef struct grade
{
	humanistic_grade_t humanistic;
	real_grade_t real;
	float sports;
} grade_t;

typedef struct student
{
	char first_name[100];
	char last_name[100];
	grade_t grades;
} student_t;

student_t students_array[ARRAY_SIZE] = 
{ 
	{"Amit", "Yehezkel", { {50, 60, 70}, {90, 95, 98}, 73.2f }},
	{"Lev", "Poliak", { {25, 73, 68}, {92, 98, 84}, 83.2f }},
	{"Sahar", "Moalem", { {85, 65, 94}, {60, 75, 48}, 93.3f }},
	{"Liran", "Yakovi", { {95, 53, 85}, {43, 68, 23}, 13.4f }},
	{"Ofir", "Nahshoni", { {28, 27, 26}, {25, 24, 23}, 63.5f }}
};

void TestStack(void)
{
	size_t i = 0;
	student_t copy;
	stack_t* stack = StackCreate(ARRAY_SIZE, sizeof(student_t));
	student_t* runner = students_array;
	assert(NULL != stack);
	
	for (; i < ARRAY_SIZE; ++i, ++runner)
	{
		StackPush(stack, runner);
	}
	
	while (!StackIsEmpty(stack))
	{
		StackPeek(stack, &copy);
		--runner;
		assert(copy.grades.sports == runner->grades.sports);
		assert(copy.grades.real.math == runner->grades.real.math);
		assert(0 == strcmp(copy.last_name, runner->last_name));
		assert(ARRAY_SIZE == StackCapacity(stack));
		StackPop(stack);
		--i;
		assert(StackSize(stack) == i); 
	}
	
	StackDestroy(stack);
}

int main(void)
{
	TestStack();
	return 0;
}

