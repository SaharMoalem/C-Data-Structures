#include <stdio.h> /* printf */
#include <string.h>  /* strcmp */
#include <assert.h> /* assert */

#include "dvector.h"

/******************************************************************************/
/***                        Color Defines   		                      ***/
/******************************************************************************/
#define DEF     "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

static int test_counter_g = 0;
static int success_counter_g = 0;

#define TEST(name, real, expected) \
    do { \
        ++test_counter_g; \
        ((real) == (expected)) ? \
        ++success_counter_g : \
        printf(RED "%s, Failed on line %d, Real: %lx, Expected: %lx\n" DEF, name, __LINE__, (unsigned long) (real), (unsigned long) (expected)); \
    } while (0)


#define PASS (test_counter_g == success_counter_g) ? \
    printf(GREEN "Passed everything!\n" DEF) : \
    printf(RED "Failed " BOLDRED "%d/%d" RED" tests.\n\n" DEF, \
           test_counter_g - success_counter_g, test_counter_g)

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

void TestDynamicVectorNormal(void)
{
	dvector_t* vector = DvectorCreate(1, sizeof(student_t));
	size_t i = 0;
	student_t copy;
	student_t* runner = students_array;
	
	for (; i < ARRAY_SIZE; ++i)
	{
		DvectorPushBack(vector, students_array + i);
	}
	
	TEST("Size vector check", DvectorSize(vector), ARRAY_SIZE);
	
	for (i = 0; i < ARRAY_SIZE; ++i)
	{
		DvectorGetElement(vector, i, &copy);
		TEST("Ensure element is like original", copy.grades.sports, runner->grades.sports);
		TEST("Ensure element is like original", copy.grades.real.math, runner->grades.real.math);
		TEST("Ensure element is like original", strcmp(copy.last_name, runner->last_name), 0);
		++runner;
	}
	
	for (i = 0; i < ARRAY_SIZE; ++i)
	{
		--runner;
		DvectorSetElement(vector, i, runner);
	}
	
	for (i = 0; i < ARRAY_SIZE; ++i)
	{
		DvectorGetElement(vector, ARRAY_SIZE - i - 1, &copy);
		TEST("Ensure element is like original", copy.grades.sports, runner->grades.sports);
		TEST("Ensure element is like original", copy.grades.real.math, runner->grades.real.math);
		TEST("Ensure element is like original", strcmp(copy.last_name, runner->last_name), 0);
		++runner;
	}
	
	while (DvectorSize(vector) > 0)
	{
		DvectorPopBack(vector);
	}
	
	TEST("Check popback", DvectorSize(vector), 0);
	DvectorResize(vector, 100);
	TEST("Test resize", DvectorCapacity(vector), 100);
	DvectorShrink(vector);
	TEST("Test shrink", DvectorCapacity(vector), 0);
	DvectorDestroy(vector);
}

int main(void)
{
	TestDynamicVectorNormal();
	PASS;
	return 0;
}
