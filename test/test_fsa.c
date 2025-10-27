#include <stdio.h> 
#include <stdlib.h> 

#include "fsa.h"
#include "test_macros.h"

void FSATest()
{
	fsa_t* fsa = NULL;
	void* memory = malloc(10);
	size_t* arr[20] = {0};
	size_t i = 0;

	TEST("Test FSASuggestSize",FSASuggestSize(5, 10) % 8, 0);

	fsa = FSAInit(memory, 1, 10);
	TEST("Test FSAInit failure", fsa, NULL);
	memory = malloc(FSASuggestSize(1, 20));
	fsa = FSAInit(memory, 1, FSASuggestSize(1, 20));
	TEST("Test FSAInit and FSACountFree", FSACountFree(fsa), 20);

	for( i = 0; i < 20; ++i)
	{
		arr[i] = FSAAlloc(fsa);
		*(arr[i]) = i;
	}

	TEST("Test FSAAlloc and FSACountFree", FSACountFree(fsa), 0);

	for( i = 0; i < 20; ++i)
	{
		TEST("Check value after FSAAlloc", *(arr[i]), i);
		FSAFree(fsa, arr[i]);
	}
	TEST("Test FSAFree and FSACountFree", FSACountFree(fsa), 20);
}

int main()
{
	FSATest();
	PASS;
	return 0;
}
