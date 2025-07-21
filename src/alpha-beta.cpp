// C++ program to demonstrate
// working of Alpha-Beta Pruning
// https://www.geeksforgeeks.org/dsa/minimax-algorithm-in-game-theory-set-4-alpha-beta-pruning/
#include<bits/stdc++.h>
using namespace std;
#include<GameState.h>

// Initial values of
// Alpha and Beta
const int MAX = 1000;
const int MIN = -1000;
const int MAX_DEPTH=10;
// Returns optimal value for
// current player(Initially called
// for root and maximizer)

int compute_heuristic(GameState* gameState){
    int numbWhitePieces=0;
    int numbBlackPieces=0;
    int heuristicScore=0;
    numbWhitePieces=gameState->getNumbWhitePieces();
    numbBlackPieces=gameState->getNumbBlackPieces();

    if (gameState->getCurrentPlayer()){
        heuristicScore=numbWhitePieces+numbWhitePieces-numbBlackPieces;
    } else{
        heuristicScore=numbBlackPieces+numbBlackPieces-numbWhitePieces;
    }
    return heuristicScore;
}

int minimax(int depth, int nodeIndex,
            bool maximizingPlayer,
            int values[], int alpha, 
            int beta, GameState* gamestate)
{

    // Terminating condition. i.e 
    // leaf node is reached
    if (depth == MAX_DEPTH || gamestate->isGameOver())
       
        return values[nodeIndex]+compute_heuristic(gamestate);

    if (maximizingPlayer)
    {
        int best = MIN;

        // Recur for left and 
        // right children
        for (int i = 0; i < 2; i++)
        {
            
            int val = minimax(depth + 1, nodeIndex * 2 + i, 
                              false, values, alpha, beta, gamestate);
            best = max(best, val);
            alpha = max(alpha, best);

            // Alpha Beta Pruning
            if (beta <= alpha)
                break;
        }
        return best;
    }
    else
    {
        int best = MAX;

        // Recur for left and
        // right children
        for (int i = 0; i < 2; i++)
        {
            int val = minimax(depth + 1, nodeIndex * 2 + i,
                              true, values, alpha, beta, gamestate);
            best = min(best, val);
            beta = min(beta, best);

            // Alpha Beta Pruning
            if (beta <= alpha)
                break;
        }
        return best;
    }
}