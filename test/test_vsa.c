#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc, free*/

#include "vsa.h"
#include "test_macros.h"

void TestVSA()
{
	vsa_t* vsa = NULL;
	void* memory = malloc(10);
	size_t* arr[10] = {0};
	size_t i = 0;

	vsa = VSAInit(memory, 10);
	TEST("memory size", vsa, NULL);
	free(memory);
	memory = malloc(sizeof(size_t)*34);
	vsa = VSAInit(memory, sizeof(size_t)*34);

	TEST("LargestFree",VSALargestChunkAvailable(vsa), 240);
	
	for( i = 0; i < 10; ++i)
	{
		arr[i] = VSAAlloc(vsa, sizeof(size_t));
		*(arr[i]) = i;
	}
	TEST("LargestFree",VSALargestChunkAvailable(vsa), 0);

	for( i = 0; i < 10; ++i)
	{
		TEST("Value", *(arr[i]), i);
		VSAFree(arr[i]);
	}
	TEST("LargestFree",VSALargestChunkAvailable(vsa), 240);

	free(memory);

}


int main()
{
	TestVSA();
	PASS;
	return 0;
}