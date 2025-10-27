#include <stdio.h>                 

#include "macros.h"
#include "test_structs.h"
#include "queue.h"

static void TestQueue(void)
{
    queue_t* queue = QCreate();
    size_t i = 0;
    
    TEST("Check QIsEmpty", QIsEmpty(queue), 1);
    for( ; i < ARRAY_SIZE; ++i)
    {
        QEnqueue(queue, students_array + i);
    }
    
    TEST("Check QGetSize", QGetSize(queue), ARRAY_SIZE);
    TEST("Check QPeek", QPeek(queue), students_array);
    
    QDequeue(queue);
    TEST("Check QDequeue", QPeek(queue), students_array + 1);
    TEST("Check QIsEmpty again", QIsEmpty(queue), 0);
    TEST("Check QGetSize again", QGetSize(queue), ARRAY_SIZE - 1);
    
    queue_t* queue_to_append = QCreate();
    QEnqueue(queue_to_append, students_array);
    QEnqueue(queue_to_append, students_array + 1);
    QDequeue(queue);
    QAppend(queue_to_append, queue);
    TEST("Check QAppend", QIsEmpty(queue_to_append), 1);
    TEST("Check QGetSize after append", QGetSize(queue), ARRAY_SIZE);
    
    QDestroy(queue);
    QDestroy(queue_to_append);
}

int main(void)
{
    TestQueue();
    PASS;
    return 0;
}
