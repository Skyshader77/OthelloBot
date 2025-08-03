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
    }else if (level==medium){
        MinimaxResult chosenMove=minimaxSimple(0, true, MIN, MAX, gamestate, gamestate->getCurrentPlayer());
        return chosenMove.bestMove;
    } else{
        MinimaxResult chosenMove=iterativeDeepeningSearch(gamestate, MAX_DEPTH_HARD, gamestate->getCurrentPlayer(),TIME_LIMIT);
        return chosenMove.bestMove;
    }
}