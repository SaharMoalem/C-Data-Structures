#include "test_macros.h"
#include "trie.h"

void TestTrie(void)
{
    trie_t* trie = TrieCreate(4);
    trie_status_t status = TRIE_SUCCESS;
    bit_array_t val1 = {0};
	bit_array_t val2 = {15};
    bit_array_t val3 = {10};

    status = TrieAlloc(trie, &val1);
    TEST("Alloc test", status, TRIE_SUCCESS);
    status = TrieAlloc(trie, &val2);
    TEST("Alloc test", status, TRIE_SUCCESS);
    status = TrieAlloc(trie, &val3);
    TEST("Alloc test", status, TRIE_SUCCESS);
    status = TrieAlloc(trie, &val3);
    TEST("Alloc test", status, TRIE_SUCCESS);

    TEST("Num Of Full Leaves", TrieGetNumFullLeaves(trie), 4);

    status = TrieAlloc(trie, &val2);
    TEST("Alloc test", status, TRIE_FULL);

    TEST("Num Of Full Leaves", TrieGetNumFullLeaves(trie), 4);

    status = TrieFreeLeaf(trie, &val3);
    TEST("Free test", status, TRIE_SUCCESS);

    TEST("Num Of Full Leaves", TrieGetNumFullLeaves(trie), 3);

    status = TrieFreeLeaf(trie, &val3);
    TEST("Free test", status, TRIE_DOUBLE_FREE);

    TEST("Num Of Full Leaves", TrieGetNumFullLeaves(trie), 3);

    TrieDestroy(trie);
}

int main(void)
{
    TestTrie();
    PASS;
    return 0;
}
