#include "test_macros.h"
#include "circular_buffer.h"

static void TestCircularBuffer(void)
{
    circular_buffer_t* c_buffer = CBCreate(64);
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    int cpy_num;
    
    TEST("Check CBIsEmpty", CBIsEmpty(c_buffer), 1);
    CBWrite(c_buffer, &num1, sizeof(int));
    CBWrite(c_buffer, &num2, sizeof(int));
    while (0 != CBWrite(c_buffer, &num3, sizeof(int)));
    
    TEST("Check CBGetSize", CBGetSize(c_buffer), 64);
    TEST("Check CBFreeSpace", CBFreeSpace(c_buffer), 0);
    CBRead(c_buffer, &cpy_num, sizeof(int));
    TEST("Check CBRead", cpy_num, 1);
    CBRead(c_buffer, &cpy_num, sizeof(int));
    TEST("Check CBRead", cpy_num, 2);
    
    TEST("Check CBIsEmpty", CBIsEmpty(c_buffer), 0);
    TEST("Check CBGetSize", CBGetSize(c_buffer), 56);
    
    CBRead(c_buffer, &cpy_num, sizeof(int));
    TEST("Check CBRead", cpy_num, 3);
    
    CBDestroy(c_buffer);
}

int main(void)
{
    TestCircularBuffer();
    PASS;
    return 0;
}

