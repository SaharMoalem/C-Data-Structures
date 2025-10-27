#ifndef __TEST_STRUCTS_H__
#define __TEST_STRUCTS_H__


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

#endif
