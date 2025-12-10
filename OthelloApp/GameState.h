#ifndef GAMESTATE_H
#define GAMESTATE_H


#include <string>
using namespace std;
#include "PiecePosition.h"
#include "Player.h"
#include "Board.h"
#include "BitCounter.h"
#include <memory>
#include <vector>

class GameState {
  public:
    GameState();
    GameState(shared_ptr<Board> board, int currentPlayer);
    void addHumanPlayer();
    void addComputerPlayer(enumDifficulty cpuDifficulty);
    void initiateGame();
    void insertPiece(piecePosition newPiecePosition);
    bool isGameOver() const;
    int getWinner() const;
    void printWinner() const;
    enumDifficulty selectBotDifficulty();
    GameState deepCopy();
    void changeCurrentPlayer();
    int getNumbBlackPieces() const;
    int getNumbWhitePieces() const;
    int getCurrentPlayer() const;
    shared_ptr<Board> getBoard();
    void runGame();
    vector<std::shared_ptr<Player>>& getPlayers();
  private:                   
    vector<shared_ptr<Player>> players;
    shared_ptr<Board> board;
    BitCounter bitcounter;
    int currentPlayer;  
};

#endif
