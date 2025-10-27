#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "recursion.h"
#include "test_macros.h"

static Node* CreateNode(int value)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if(!node)
    {
        printf("Couldn't test FlipList, failed allocation");
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    
    return node;
}

static void ConnectNodes(Node* node_1, Node* node_2)
{
    node_1->next = node_2;
}

static void DestroyList(Node* head)
{
    Node* tmp;
    while (NULL != head->next)
    {
        tmp = head->next;
        free(head);
        head = tmp;
    }
    free(head);
}

static void TestFibonacci()
{
    TEST("Check Fibonacci", IterFibonacci(8), 21);
    TEST("Check Fibonacci", RecFibonacci(8), 21);
    TEST("Check Fibonacci", IterFibonacci(1), 1);
    TEST("Check Fibonacci", RecFibonacci(1), 1);
    TEST("Check Fibonacci", IterFibonacci(12), 144);
    TEST("Check Fibonacci", RecFibonacci(12), 144);
    TEST("Check Fibonacci", IterFibonacci(2), 1);
    TEST("Check Fibonacci", RecFibonacci(2), 1);
}

static void TestFlipList()
{
    Node* first = CreateNode(1);
    Node* second = CreateNode(2);
    Node* third = CreateNode(3);
    Node* fourth = CreateNode(4);
    Node* fifth = CreateNode(5);
    Node* current = first;
    int i = 5;
    
    ConnectNodes(first, second);
    ConnectNodes(second, third);
    ConnectNodes(third, fourth);
    ConnectNodes(fourth, fifth);
    
    current = FlipList(current);
    while (NULL != current)
    {
        TEST("Check Flip", current->value, i);
        current = current->next;
        --i;
    }

    DestroyList(fifth);
}

static void TestSortStack()
{
    stack_t* stack = StackCreate(6, sizeof(int));
    int values[] = {44, 0, -13, 23, -7, 100};
    int sorted_values[] = {100, 44, 23, 0, -7, -13};
    int curr_value = 0;
    size_t i = 0;

    if(!stack)
    {
        printf("Couldn't test SortStack, failed allocation");
        return;
    }

    for(; i < 6; ++i)
    {
        StackPush(stack, values + i);
    }

    SortStack(stack);
    
    for(i = 0; i < 6; ++i)
    {
        StackPeek(stack, &curr_value);
        TEST("Check SortStack", curr_value, *(sorted_values + i));
        StackPop(stack);
    }

    free(stack);
}

static void TestStrlen()
{
    TEST("Check Strlen", Strlen("My length is 15"), 15);
    TEST("Check Strlen", Strlen(""), 0);
    TEST("Check Strlen", Strlen("Werewolf"), 8);
}

static void TestStrcmp()
{
    TEST("Check Strcmp", Strcmp("Werewolf", "Werewolf"), 0);
    TEST("Check Strcmp", Strcmp("Werewolf", "Seer") > 0, 1);
    TEST("Check Strcmp", Strcmp("Robber", "Seer") < 0, 1);
}

static void TestStrcpycat()
{
    const char* src1 = "Mason";
    const char* src2 = "Troublemaker";
    char* dst1 = malloc((Strlen(src1) + Strlen(src2) + 1));
    char* dst2 = malloc((Strlen(src2) + 1));

    dst1 = Strcpy(dst1, src1);
    dst2 = Strcpy(dst2, src2);
    TEST("Check Strcpy", Strcmp(dst1, src1), 0);
    TEST("Check Strcpy", Strcmp(dst2, src2), 0);

    dst1 = Strcat(dst1, src2);
    TEST("Check Strcat", Strcmp(dst1, "MasonTroublemaker"), 0);

    free(dst1);
    free(dst2);
}

static void TestStrstr()
{
    char* result1 = Strstr("It's like a needle inside haystack", "needle");
    char* result2 = Strstr("It's like a needle inside haystack", "not needle");

    TEST("Check Strstr", Strcmp(result1, "needle inside haystack"), 0);
    TEST("Check Strstr", result2, NULL);
}

int main(void)
{
    TestFibonacci();
    TestFlipList();
    TestSortStack();
    TestStrlen();
    TestStrcmp();
    TestStrcpycat();
    TestStrstr();
    PASS;
    return 0;
}