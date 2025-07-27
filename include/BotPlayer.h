#ifndef BOT_H
#define BOT_H

#include <string>
using namespace std;
#include <PiecePosition.h>
#include <Player.h>
#include <GameState.h>

class BotPlayer: public Player  {
public:
    BotPlayer(int color, enumDifficulty difficulty) : Player(color), level(difficulty) {};
    piecePosition pickAction(GameState* gamestate);
private:
    enumDifficulty level;
};

#endif