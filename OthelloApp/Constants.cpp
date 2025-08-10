#include "Constants.h"
#include <vector>

int BOARD_LENGTH = 8;
int NUMB_COLORS = 2;
string WHITE = "W ";
string BLACK = "B ";
string EMPTY = ". ";
uint64_t FULL_BOARD = 0xFFFFFFFFFFFFFFFFULL;
int DIRECTIONS[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
int MAX = 1000;
int MIN = -1000;
int MAX_DEPTH = 5;
int TIME_LIMIT=600000;
int MAX_DEPTH_HARD=8;
int CORNER_VALUE=99;

vector<pair<int, int>> CORNERS = {
        {0, 0},                              
        {BOARD_LENGTH - 1, 0},              
        {0, BOARD_LENGTH - 1},               
        {BOARD_LENGTH - 1, BOARD_LENGTH - 1}
};

vector<pair<int, int>> ORTHOGONAL_OFFSETS = {
                    {-1, 0}, 
                    {1, 0},   
                    {0, -1},  
                    {0, 1}    
};

string WHITE_IMAGE="White.png";
string BLACK_IMAGE="Black.png";
