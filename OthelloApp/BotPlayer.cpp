#include "BotPlayer.h"
#include "Alpha-beta.h"
#include "MCTSAlgo.h"
#include "GreedyPlayer.h"
#include <iostream>
using namespace std;
#include <vector>
#include "Constants.h"

piecePosition BotPlayer::pickAction(GameState* gamestate) {
    int currentPlayerColor = gamestate->getCurrentPlayer();
    if (level == easy) {
        GreedyResult chosenMove = greedyAlgo(gamestate);
        return chosenMove.bestMove;
    }
    
    piecePosition cornerAction = pickCornerAction(gamestate);
    
    if (cornerAction.xCoord != -1) {
        cornerAction.ncolor = currentPlayerColor;
        return cornerAction;
    }
    
    piecePosition adjacentToCornerAction = pickAdjacentToCornerAction(gamestate);

    if (adjacentToCornerAction.xCoord != -1) {
        adjacentToCornerAction.ncolor = currentPlayerColor;
        return adjacentToCornerAction;
    }

    if (level == medium) {
        MinimaxResult chosenMove;
        chosenMove = minimaxSimple(0, true, MIN, MAX, gamestate, currentPlayerColor);
        return chosenMove.bestMove;
    } else if(level == hard) {
        BestAction bestActionMCTS= mctsSearchTimed(gamestate, MAX_ITERATIONS_MCTS, TIME_LIMIT_MCSTS);
        return bestActionMCTS.action;
        
    } else{
        MinimaxResult chosenMove;
        chosenMove = iterativeDeepeningSearch(gamestate, MAX_DEPTH_HARD, currentPlayerColor, TIME_LIMIT);
        return chosenMove.bestMove;
    }
    
}

piecePosition BotPlayer::pickCornerAction(GameState* gamestate){
    vector<piecePosition> availableCorners = gamestate->getBoard()->getCornerEmptySpaces();

    if (availableCorners.size() > 0) {
        int randomIndex = rand() % availableCorners.size();
        return availableCorners[randomIndex];
    }
    
    return createInvalidPosition();
}

piecePosition BotPlayer::pickAdjacentToCornerAction(GameState* gamestate){
    vector<piecePosition> availablePositions = gamestate->getBoard()->getEmptyTilesOrthogonalToSameColorCorners(gamestate->getCurrentPlayer());
    if (availablePositions.size() > 0) {
        int randomIndex = rand() % availablePositions.size();
        return availablePositions[randomIndex];
    }

    return createInvalidPosition();
}

piecePosition BotPlayer::createInvalidPosition() const {
    piecePosition invalidPos;
    invalidPos.xCoord = -1;
    invalidPos.yCoord = -1;
    invalidPos.ncolor = -1;
    return invalidPos;
}