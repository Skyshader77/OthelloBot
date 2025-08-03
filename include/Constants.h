#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstdint>
#include <string>

using namespace std;

extern int BOARD_LENGTH;
extern int NUMB_COLORS;
extern string WHITE;
extern string BLACK;
extern string EMPTY;
extern uint64_t FULL_BOARD;
extern int DIRECTIONS[8][2];
extern int MAX;
extern int MIN;
extern int MAX_DEPTH;
extern int TIME_LIMIT;
extern int MAX_DEPTH_HARD;

enum enumPiece
{
    nWhite,
    nBlack,
};

enum enumDifficulty
{
    easy,
    medium,
    hard,
};

// Constants for minimax algorithm


#endif