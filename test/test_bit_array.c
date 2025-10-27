#include <string.h>        /* strcmp */
#include <stdio.h>         /* printf */
#include <stdlib.h>        /* malloc, free */

#include <bit_array.h>     /* function decleration */

#define BIT_ARRAY_SIZE (64)
#define MAX_ARRAY_VALUE (0xFFFFFFFFFFFFFFFF)

int CheckBitArraySetAll()
{
    bit_array_t test_array = 6;
    return BitArraySetAll(test_array) == MAX_ARRAY_VALUE;
}

int CheckBitArrayResetAll()
{
    bit_array_t test_array = 6;
    return BitArrayResetAll(test_array) == 0;
}

int CheckBitArraySetBitOn()
{
    bit_array_t test_array = 6;
    return 38 == BitArraySetBitOn(test_array, 5);
}

int CheckBitArraySetBitOff()
{
    bit_array_t test_array = 38;
    return 36 == BitArraySetBitOff(test_array, 1);
}

int CheckBitArraySetBit()
{
    bit_array_t test_array = 36;
    return 292 == BitArraySetBit(test_array, 8, 1);
}

int CheckBitArrayGetValue()
{
    bit_array_t test_array = 292;
    return 0 == BitArrayGetValue(test_array, 4) && 1 == BitArrayGetValue(test_array, 5);
}

int CheckBitArrayMirror()
{
    bit_array_t test_array = 292;
    return 0x2480000000000000 == BitArrayMirror(test_array);
}

int CheckBitArrayFlipBit()
{
    bit_array_t test_array = 292;
    return 290 == BitArrayFlipBit(BitArrayFlipBit(test_array, 2), 1);
}

int CheckBitArrayRotateRight()
{
    bit_array_t test_array = 0xFFFFFFFFFFFFFFFD;
    return 0x7FFFFFFFFFFFFFFF == BitArrayRotateRight(test_array, 2) &&
              0xFF7FFFFFFFFFFFFF ==BitArrayRotateRight(test_array, 10);
}

int CheckBitArrayRotateLeft()
{
    bit_array_t test_array = 0xFFFFFFFFFFFFFFFD;
    return 0x7FFFFFFFFFFFFFFF == BitArrayRotateLeft(test_array, 62) &&
              0xFF7FFFFFFFFFFFFF ==BitArrayRotateLeft(test_array, 54);
}

int CheckBitArrayCountOn()
{
    bit_array_t test_array = 0xFFFFFFFFFFFFFFFD;
    return 63 == BitArrayCountOn(test_array);
}

int CheckBitArrayCountOff()
{
    bit_array_t test_array = 0xFFFFFFFFFFFFFFFD;
    return 1 == BitArrayCountOff(test_array);
}

int CheckBitArrayToString()
{
    char* to_string = malloc(BIT_ARRAY_SIZE + 1);
    bit_array_t test_array = 456;
    int exit_status;
    
    BitArrayToString(test_array, to_string);
    exit_status = 0 == strcmp(to_string, "0000000000000000000000000000000000000000000000000000000111001000");
    free(to_string);
    
    return exit_status;
}

int main()
{
    if (CheckBitArraySetAll())
    {
        printf("BitArraySetAll success!\n");
    }
    if (CheckBitArrayResetAll())
    {
        printf("BitArrayResetAll success!\n");
    }
    if (CheckBitArraySetBitOn())
    {
        printf("BitArraySetBitOn success!\n");
    }
    if (CheckBitArraySetBitOff())
    {
        printf("BitArraySetBitOff success!\n");
    }
    if (CheckBitArraySetBit())
    {
        printf("BitArraySetBit success!\n");
    }
    if (CheckBitArrayGetValue())
    {
        printf("BitArrayGetValue success!\n");
    }
    if (CheckBitArrayMirror())
    {
        printf("BitArrayMirror success!\n");
    }
    if (CheckBitArrayFlipBit())
    {
        printf("BitArrayFlipBit success!\n");
    }
    if (CheckBitArrayRotateRight())
    {
        printf("BitArrayRotateRight success!\n");
    }
    if (CheckBitArrayRotateLeft())
    {
        printf("BitArrayRotateLeft success!\n");
    }
    if (CheckBitArrayCountOn())
    {
        printf("BitArrayCountOn success!\n");
    }
    if (CheckBitArrayCountOff())
    {
        printf("BitArrayCountOff success!\n");
    }
    if (CheckBitArrayToString())
    {
        printf("BitArrayToString success!\n");
    }
    
    return 0;
}

