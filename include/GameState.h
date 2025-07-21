#ifndef GAMESTATE_H
#define GAMESTATE_H


#include <string>
using namespace std;
#include <PiecePosition.h>
#include <Player.h>
#include <Board.h>
#include <BitCounter.h>
#include <memory>

class GameState {
  public:
    GameState();
    void insertPiece(piecePosition newPiecePosition);
    bool isGameOver() const;
    bool printWinner() const;
    int getNumbBlackPieces() const;
    int getNumbWhitePieces() const;
    int getCurrentPlayer() const;
    shared_ptr<Board> getBoard() const;
    void runGame();
  private:                   
    Player players[2];
    shared_ptr<Board> board;
    BitCounter bitcounter;
    int currentPlayer;  
};

#endif