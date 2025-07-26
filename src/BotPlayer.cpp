#include "BotPlayer.h"
#include "Alpha-beta.h"
#include <iostream>
using namespace std;
#include <vector>
#include "Constants.h"

piecePosition BotPlayer::pickAction(GameState* gamestate){
    MinimaxResult chosenMove=minimax(MAX_DEPTH, true, MAX, MIN, gamestate);
    return chosenMove.bestMove;
}