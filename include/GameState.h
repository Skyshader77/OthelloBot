#ifndef GAMESTATE_H
#define GAMESTATE_H


#include <string>
using namespace std;
#include <PiecePosition.h>
#include <Player.h>
#include <Board.h>
#include <BitCounter.h>
#include <memory>
#include <vector>

class GameState {
  public:
    GameState();
    void addHumanPlayer();
    void addComputerPlayer();
    void initiateGame();
    void insertPiece(piecePosition newPiecePosition);
    bool isGameOver() const;
    void printWinner() const;
    GameState deepCopy();
    void changeCurrentPlayer();
    int getNumbBlackPieces() const;
    int getNumbWhitePieces() const;
    int getCurrentPlayer() const;
    shared_ptr<Board> getBoard();
    void runGame();
  private:                   
    vector<shared_ptr<Player>> players;
    shared_ptr<Board> board;
    BitCounter bitcounter;
    int currentPlayer;  
};

#endif