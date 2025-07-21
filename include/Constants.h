#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstdint>
#include <string>

using namespace std;

extern int BOARD_SIZE;
extern int NUMB_COLORS;
extern string WHITE;
extern string BLACK;
extern string EMPTY;
extern uint64_t FULL_BOARD;
extern int DIRECTIONS[8][2];

enum enumPiece
{
    nWhite,
    nBlack,
};

#endif