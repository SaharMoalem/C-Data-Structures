#include <string.h>
#include <stdlib.h>

#include "test_macros.h"
#include "hash_table.h"

static size_t DictHash(const void* data)
{
	const unsigned char* str = (const unsigned char*)data;
	size_t hash = 5381;

	while('\0' != *str)
	{
		hash = ((hash << 5) + hash) + *str;
		++str;
	}

	return hash;
}


static int IsSameDict(const void* data, const void* param)
{
	return strcmp(data, param) == 0;
}

static void LoadDictionary(void)
{
	FILE* file = fopen("/usr/share/dict/words", "r");
	char arr[256];
	char* arr2[120000] = {NULL};
	size_t i = 0;
	hash_table_t* table = HTCreate(26*26*26*2, DictHash, IsSameDict);

	while (fgets(arr, 256, file) != NULL)
	{
		arr2[i] = (char*)malloc(strlen(arr) + 1);
		strcpy(arr2[i], arr);
		HTInsert(table, arr2[i]);
		++i;
	}

	fclose(file);
	printf("Enter word to check\n");

	if(fgets(arr, 256, stdin) == NULL)
	{
		return;
	}

	if (!HTFind(table, arr))
	{
		printf("Wrong word\n");
	}
	else
	{
		printf("Correct word\n");
	}

	printf("Load factor: %.5f SD: %.5f\n", HTLoadFactor(table), HTGetSD(table));

	HTDestroy(table);

	for (;i > 0; --i)
	{
		free(arr2[i - 1]);
	}
}

int main(void)
{
	LoadDictionary();
	PASS;
	return 0;
}
