#include <stdio.h>
#include <sys/types.h>

#include "knight_tour.h"
#include "bit_array.h"

#define NUM_OF_STEPS (8)

typedef struct point {
    size_t row;
    size_t col;
} Point;

static Point board_lut[NUM_OF_ROW][NUM_OF_COL][NUM_OF_STEPS];
static size_t row_move[NUM_OF_ROW] = {-2, -1, 1, 2, 2, 1, -1, -2};
static size_t col_move[NUM_OF_COL] = {1, 2, 2, 1, -1, -2, -2, -1};

static int IsValidPoint(size_t board[NUM_OF_ROW][NUM_OF_COL], Point point)
{
    return point.col < NUM_OF_COL && point.row < NUM_OF_ROW &&
                                            board[point.row][point.col] == 0;
}

static void InitLUT(void)
{
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;

    for( ; i < NUM_OF_ROW; ++i)
    {
        for(j = 0; j < NUM_OF_COL; ++j)
        {
            for(k = 0; k < NUM_OF_STEPS; ++k)
            {
                board_lut[i][j][k].row = i + row_move[k];
                board_lut[i][j][k].col = j + col_move[k];
            }
        }
    } 
}

static size_t CountOptions(size_t board[NUM_OF_ROW][NUM_OF_COL], size_t row,
                                                                    size_t col)
{
    size_t count_moves = 0;
    size_t i = 0;

    for( ; i < NUM_OF_STEPS; ++i)
    {
        if(IsValidPoint(board, board_lut[row][col][i]))
        {
            ++count_moves;
        }
    }

    return count_moves;
}

static size_t FindBestOption(size_t board[NUM_OF_ROW][NUM_OF_COL], size_t row,
                                                                    size_t col)
{
    size_t global_min_move = NUM_OF_STEPS;
    size_t curr_move = 0;
    size_t best_step = 0;
    size_t i = 0;

    for( ; i < NUM_OF_STEPS; ++i)
    {
        if(IsValidPoint(board, board_lut[row][col][i]))
        {
            curr_move = CountOptions(board, board_lut[row][col][i].row,
                                                    board_lut[row][col][i].col);
            if(curr_move < global_min_move)
            {
                global_min_move = curr_move;
                best_step = i;
            }
        }
    }

    return best_step;
}

static void Heuristic(size_t board[NUM_OF_ROW][NUM_OF_COL], size_t start_row,
                                            size_t start_col, bit_array_t steps)
{
    size_t next_row = 0;
    size_t next_col = 0;
    size_t best_move = NUM_OF_STEPS;

    board[start_row][start_col] = BitArrayCountOn(steps) + 1;

    if(board[start_row][start_col] == NUM_OF_ROW * NUM_OF_COL)
    {
        return;
    }

    best_move = FindBestOption(board, start_row, start_col);
    next_row = board_lut[start_row][start_col][best_move].row;
    next_col = board_lut[start_row][start_col][best_move].col;
    Heuristic(board, next_row, next_col,
        BitArraySetBitOn(steps, (next_row * NUM_OF_ROW) + next_col));
}

static int Backtracking(size_t board[NUM_OF_ROW][NUM_OF_COL], size_t start_row,
                                            size_t start_col, bit_array_t steps)
{
    size_t next_row = 0;
    size_t next_col = 0;
    size_t i = 0;

    board[start_row][start_col] = BitArrayCountOn(steps) + 1;

    if(board[start_row][start_col] == NUM_OF_ROW * NUM_OF_COL)
    {
        return 1;
    }

    for( ; i < NUM_OF_STEPS; ++i)
    {
        if(IsValidPoint(board, board_lut[start_row][start_col][i]))
        {
            next_row = board_lut[start_row][start_col][i].row;
            next_col = board_lut[start_row][start_col][i].col;

            if(Backtracking(board, next_row, next_col, BitArraySetBitOn(steps,
                                        (next_row * NUM_OF_ROW) + next_col)))
            {
                return 1;
            }

            else
            {
                board[next_row][next_col] = 0;
            }
        }
    }

    return 0;
}

void KnightTour(size_t board[NUM_OF_ROW][NUM_OF_COL], size_t start_row,
                                            size_t start_col, int is_heuristics)
{
    InitLUT();

    if(is_heuristics)
    {
        Heuristic(board, start_row, start_col, 0);
    }

    else
    {
        Backtracking(board, start_row, start_col, 0);
    }
}

void PrintBoard(size_t board[NUM_OF_ROW][NUM_OF_COL])
{
    size_t i = 0;
    size_t j = 0;

    for( ; i < NUM_OF_ROW; ++i)
    {
        for(j = 0; j < NUM_OF_COL; ++j)
        {
            printf("%lu\t", board[i][j]);
        }
        
        printf("\n");
    }
}