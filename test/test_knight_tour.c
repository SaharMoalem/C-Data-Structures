#include <stdio.h>

#include "knight_tour.h"

static void TestKnightTour()
{
    size_t backtrack_board[NUM_OF_ROW][NUM_OF_COL] = {0};
    size_t heuristic_board[NUM_OF_ROW][NUM_OF_COL] = {0};
    
    KnightTour(backtrack_board, 7, 3, 0);
    PrintBoard(backtrack_board);
    printf("\n\n");

    KnightTour(heuristic_board, 4, 7, 1);
    PrintBoard(heuristic_board);
}

int main(void)
{
    TestKnightTour();

    return 0;
}