#ifndef MCTS_ALGO_H
#define MCTS_ALGO_H

#include "Constants.h"
#include "Board.h"
#include "PiecePosition.h"
#include "GameState.h"
#include"Logger.h"

struct BestAction {
    piecePosition action;
    int visits;
    float winRate;
    bool hasAction;
    int iterationsCompleted;
};

BestAction mctsSearchTimed(GameState* gameState, int maxIterations, int timeLimit);

#endif