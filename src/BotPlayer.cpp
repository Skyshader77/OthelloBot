#include "BotPlayer.h"
#include "Alpha-beta.h"
#include "GreedyPlayer.h"
#include <iostream>
using namespace std;
#include <vector>
#include "Constants.h"

piecePosition BotPlayer::pickAction(GameState* gamestate){
    if (level==easy){
        GreedyResult chosenMove=greedyAlgo(gamestate);
        return chosenMove.bestMove;
    }else{
        MinimaxResult chosenMove=minimax(0, true, MAX, MIN, gamestate);
        return chosenMove.bestMove;
    }
}