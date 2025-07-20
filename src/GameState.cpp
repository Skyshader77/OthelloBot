#include "GameState.h"
#include "Constants.h"
#include <iostream>
using namespace std;
#include <iostream>
#include <vector>

GameState::GameState() : players{Player(0), Player(1)}, board(), bitcounter()
{
    bitcounter.initialize();
}

bool GameState::isGameOver() const{
    return board.isFull();
}

void GameState::runGame(){
    board.resetBoard();
    piecePosition newPiecePosition;
    while (!isGameOver()){
        for (auto & player:players){
            newPiecePosition=player.pickAction();
            while (board.isInValidPosition(newPiecePosition)){
                newPiecePosition=player.insertPiece();
            }
            board.updateBoard(newPiecePosition);
            board.printBoard();
        }
    }
    printWinner();
}


bool GameState::printWinner() const{
    int numbBlackPieces= bitcounter.countSetBits(board.getBlackPieces());
    int numbWhitePieces= bitcounter.countSetBits(board.getBlackPieces());

    if (numbWhitePieces> numbBlackPieces){
        cout<< "White wins";
    }
    else if(numbBlackPieces > numbWhitePieces){
        cout<< "Black wins";
    }
    else{
        cout<< "It is a tie!";
    }
}