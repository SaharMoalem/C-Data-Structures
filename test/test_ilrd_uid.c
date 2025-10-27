#include <pthread.h>

#include "test_macros.h"
#include "ilrd_uid.h"

#define THREAD_COUNT 10
#define NUM_OF_UIDS 1000

void* ThreadTestUID(void* arg)
{
	size_t i = 0;
	ilrd_uid_t* uids = (ilrd_uid_t*)arg;
	(void)arg;
	for (i = 0; i < (NUM_OF_UIDS/THREAD_COUNT); ++i)
	{
		uids[i] = UIDCreate();
	}

	return NULL;
}

void TestResult(ilrd_uid_t* uids)
{
	size_t i = 0;
	size_t j = 0;

	for (i = 0; i < NUM_OF_UIDS; ++i)
	{
		for (j = 0; j < NUM_OF_UIDS; ++j)
		{
			if (i != j)
			{
				TEST("Compare 1 and 2", UIDIsSame(uids[i], uids[j]), 0);
			}
		}
	}
}

void TestUID(void)
{
	ilrd_uid_t uid1 = UIDCreate();
	ilrd_uid_t uid2 = UIDCreate();
	ilrd_uid_t uid3 = UIDCreate();
	ilrd_uid_t uid4 = uid1;
	pthread_t threads[THREAD_COUNT];
	ilrd_uid_t uids[NUM_OF_UIDS];
	size_t i = 0;
	
	TEST("Compare 1 and 2", UIDIsSame(uid1, uid2), 0);
	TEST("Compare 1 and 3", UIDIsSame(uid1, uid3), 0);
	TEST("Compare 1 and 4", UIDIsSame(uid1, uid4), 1);
	TEST("Compare 2 and 3", UIDIsSame(uid2, uid3), 0);
	TEST("Compare 3 and bad", UIDIsSame(uid3, bad_uid), 0);

	for (i = 0; i < THREAD_COUNT; ++i)
	{
		pthread_create(&threads[i], NULL, ThreadTestUID, 
						(uids + i*(NUM_OF_UIDS/THREAD_COUNT)));
	}
	for (i = 0; i < THREAD_COUNT; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	TestResult(uids);
	
}

int main(void)
{
	TestUID();
	PASS;

	return 0;
}
