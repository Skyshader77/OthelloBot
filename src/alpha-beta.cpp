// C++ program to demonstrate
// working of Alpha-Beta Pruning
// https://www.geeksforgeeks.org/dsa/minimax-algorithm-in-game-theory-set-4-alpha-beta-pruning/
#include<bits/stdc++.h>
using namespace std;
#include<GameState.h>
#include<Alpha-beta.h>

// Returns optimal value for
// current player(Initially called
// for root and maximizer)

int compute_heuristic(GameState* gameState, int originalColor){
    int numbWhitePieces=0;
    int numbBlackPieces=0;
    int heuristicScore=0;
    numbWhitePieces=gameState->getNumbWhitePieces();
    numbBlackPieces=gameState->getNumbBlackPieces();

    if (originalColor==nWhite){
        heuristicScore=numbWhitePieces-numbBlackPieces;
    } else{
        heuristicScore=numbBlackPieces-numbWhitePieces;
    }
    return heuristicScore;
}


MinimaxResult minimax(int depth, bool maximizingPlayer, int alpha, int beta, GameState* gamestate, int originalColor) {
    if (depth == MAX_DEPTH || gamestate->isGameOver()) {
        MinimaxResult result;
        result.value = compute_heuristic(gamestate, originalColor);
        result.hasBestMove = false;
        return result;
    }
    
    vector<piecePosition> listPossibleMoves = gamestate->getBoard()->getEmptySpaces();
    if (maximizingPlayer) {
        int best = MIN;
        piecePosition bestMove;
        bool foundMove = false;
        
        for (auto move = listPossibleMoves.begin(); move != listPossibleMoves.end(); ++move) {
            piecePosition newmove = *move;
            newmove.ncolor = gamestate->getCurrentPlayer();
            GameState newGameState = gamestate->deepCopy();
            newGameState.changeCurrentPlayer();
            newGameState.getBoard()->updateBoard(newmove);
            
            MinimaxResult result = minimax(depth + 1, false, alpha, beta, &newGameState, originalColor);
            
            if (result.value > best) {
                best = result.value;
                bestMove = newmove;
                foundMove = true;
            }
            
            alpha = max(alpha, best);
            
            // Alpha Beta Pruning
            if (beta <= alpha)
                break;
        }
        
        MinimaxResult finalResult;
        finalResult.value = best;
        finalResult.bestMove = bestMove;
        finalResult.hasBestMove = foundMove;
        return finalResult;
    }
    else {
        int best = MAX;
        piecePosition bestMove;
        bool foundMove = false;
        
        for (auto move = listPossibleMoves.begin(); move != listPossibleMoves.end(); ++move) {
            piecePosition newmove = *move;
            newmove.ncolor = gamestate->getCurrentPlayer();
            GameState newGameState = gamestate->deepCopy();
            newGameState.changeCurrentPlayer();
            newGameState.getBoard()->updateBoard(newmove);
            
            MinimaxResult result = minimax(depth + 1, true, alpha, beta, &newGameState, originalColor);
            
            if (result.value < best) {
                best = result.value;
                bestMove = newmove;
                foundMove = true;
            }
            
            beta = min(beta, best);
            
            if (beta <= alpha)
                break;
        }
        
        MinimaxResult finalResult;
        finalResult.value = best;
        finalResult.bestMove = bestMove;
        finalResult.hasBestMove = foundMove;

        return finalResult;
    }
}