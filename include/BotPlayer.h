#ifndef BOT_H
#define BOT_H

#include <string>
using namespace std;
#include <PiecePosition.h>
#include <Player.h>
#include <GameState.h>

class BotPlayer: public Player  {
public:
    BotPlayer(int color) : Player(color) {};
    piecePosition pickAction(GameState* gamestate);
};

#endif