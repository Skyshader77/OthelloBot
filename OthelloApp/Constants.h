#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstdint>
#include <string>
#include <vector>
#include <utility>
using namespace std;

extern int BOARD_LENGTH;
extern int NUMB_COLORS;
extern string WHITE;
extern string BLACK;
extern string EMPTY;
extern uint64_t FULL_BOARD;
extern string BLACK_IMAGE;
extern string WHITE_IMAGE;
extern int DIRECTIONS[8][2];
extern int MAX;
extern int MIN;
extern int MAX_DEPTH;
extern int TIME_LIMIT;
extern int TIME_LIMIT_MCSTS;
extern int MAX_DEPTH_HARD;
extern int CORNER_VALUE;
extern int DRAW_VALUE;
extern int MAX_ITERATIONS_MCTS;
extern float EXPLORE_PARAMETER;
extern vector<pair<int, int>> CORNERS;
extern vector<pair<int, int>> ORTHOGONAL_OFFSETS;
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

enum SpaceType {
    CORNER_SPACES,
    EDGE_SPACES, 
    CENTER_SPACES,
    ALL_SPACES
};

#endif
