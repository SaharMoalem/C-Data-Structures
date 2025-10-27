#ifndef __KNIGHT_TOUR_H__
#define __KNIGHT_TOUR_H__

#include <stddef.h>  /* size_t */

#define NUM_OF_ROW (8)
#define NUM_OF_COL (8)

void KnightTour(size_t board[NUM_OF_ROW][NUM_OF_COL], size_t start_row, size_t start_col, int is_heuristics);

void PrintBoard(size_t board[NUM_OF_ROW][NUM_OF_COL]);

#endif  /*__KNIGHT_TOUR__*/