#include "GameState.h"
#include <iostream>
using namespace std;
#include <iostream>
#include <vector>

GameState::GameState() : players{Player(0), Player(1)}, board(), bitcounter()
{
    bitcounter.initialize();
}

shared_ptr<Board> GameState::getBoard() const{
    return board;
}

bool GameState::isGameOver() const{
    return board->isFull();
}

int GameState::getCurrentPlayer() const{
    return currentPlayer;
}

void GameState::runGame(){
    board->resetBoard();
    piecePosition newPiecePosition;
    while (!isGameOver()){
        for (auto & player:players){
            currentPlayer=player.getColor();
            board->printBoard();
            newPiecePosition=player.pickAction();
            while (board->isInValidPosition(newPiecePosition)){
                cout<< "You have entered an invalid position." <<endl;
                newPiecePosition=player.insertPiece();
            }
            board->updateBoard(newPiecePosition);
        }
    }
    printWinner();
}

int GameState::getNumbBlackPieces() const{
    return bitcounter.countSetBits(board->getBlackPieces());
}

int GameState::getNumbWhitePieces() const{
    return bitcounter.countSetBits(board->getWhitePieces());
}

bool GameState::printWinner() const{
    int numbBlackPieces= bitcounter.countSetBits(board->getBlackPieces());
    int numbWhitePieces= bitcounter.countSetBits(board->getBlackPieces());

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