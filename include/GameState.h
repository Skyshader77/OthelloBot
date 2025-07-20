#ifndef GAMESTATE_H
#define GAMESTATE_H


#include <string>
using namespace std;
#include <PiecePosition.h>
#include <Player.h>
#include <Board.h>
#include <BitCounter.h>

class GameState {
  public:
    GameState();
    void insertPiece(piecePosition newPiecePosition);
    bool isGameOver() const;
    bool printWinner() const;
    void runGame();
  private:                   
    Player players[2];
    Board board;
    BitCounter bitcounter;  
};

#endif