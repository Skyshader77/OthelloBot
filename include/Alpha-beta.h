#ifndef MINIMAX_H
#define MINIMAX_H

#include "GameState.h"
#include "PiecePosition.h"

struct MinimaxResult {
    int value;
    piecePosition bestMove;
    bool hasBestMove;
};


/**
 * Computes heuristic value for the current game state
 * @param gameState Pointer to the current game state
 * @return Heuristic score for the current player
 */
int compute_heuristic(GameState* gameState);

/**
 * Minimax algorithm with alpha-beta pruning
 * @param depth Current depth in the search tree
 * @param maximizingPlayer True if current player is maximizing, false if minimizing
 * @param alpha Alpha value for alpha-beta pruning
 * @param beta Beta value for alpha-beta pruning
 * @param gamestate Pointer to the current game state
 * @return MinimaxResult containing the best value and move
 */
MinimaxResult minimax(int depth, bool maximizingPlayer, int alpha, int beta, GameState* gamestate);

#endif // MINIMAX_H