// C++ program to demonstrate
// working of Alpha-Beta Pruning
// https://www.geeksforgeeks.org/dsa/minimax-algorithm-in-game-theory-set-4-alpha-beta-pruning/
#include<bits/stdc++.h>
using namespace std;
#include"GameState.h"
#include"GreedyPlayer.h"

// Returns optimal value for
// current player(Initially called
// for root and maximizer)

int compute_numb_pieces(GameState* gameState){
    int numbWhitePieces=0;
    int numbBlackPieces=0;
    int heuristicScore=0;
    numbWhitePieces=gameState->getNumbWhitePieces();
    numbBlackPieces=gameState->getNumbBlackPieces();

    if (gameState->getCurrentPlayer()==nWhite){
        heuristicScore=numbWhitePieces-numbBlackPieces;
    } else{
        heuristicScore=numbBlackPieces-numbWhitePieces;
    }
    return heuristicScore;
}

GreedyResult greedyAlgo(GameState* gamestate) {
    if (gamestate->isGameOver()) {
        GreedyResult result;
        result.value = compute_numb_pieces(gamestate);
        return result;
    }
    
    vector<piecePosition> listPossibleMoves = gamestate->getBoard()->getEmptySpacesSortedByEdgeProximity();
    
    int best = MIN;
    piecePosition bestMove;    

    for (auto move = listPossibleMoves.begin(); move != listPossibleMoves.end(); ++move) {
        piecePosition newmove = *move;
        newmove.ncolor = gamestate->getCurrentPlayer();
        GameState newGameState = gamestate->deepCopy();
        newGameState.changeCurrentPlayer();
        newGameState.getBoard()->updateBoard(newmove);
            
        int greedyHeuristicValue = compute_numb_pieces(gamestate);
            
        if (greedyHeuristicValue > best) {
            best = greedyHeuristicValue;
            bestMove = newmove;
        }
    }
        
    GreedyResult finalResult;
    finalResult.value = best;
    finalResult.bestMove = bestMove;
    return finalResult;
}
