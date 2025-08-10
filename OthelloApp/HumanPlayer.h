#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include <string>
using namespace std;
#include "PiecePosition.h"
#include "Player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer(int color) : Player(color) {};
    
    piecePosition pickAction();
    
    piecePosition insertPiece();
    void leaveGame();
    
};
#endif
