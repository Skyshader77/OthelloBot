#include "Constants.h"

int BOARD_LENGTH = 8;
int NUMB_COLORS = 2;
string WHITE = "W ";
string BLACK = "B ";
string EMPTY = ". ";
uint64_t FULL_BOARD = 0xFFFFFFFFFFFFFFFFULL;
int DIRECTIONS[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
int MAX = 1000;
int MIN = -1000;
int MAX_DEPTH = 6;
int TIME_LIMIT=60000;
int MAX_DEPTH_HARD=7;