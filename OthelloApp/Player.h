#ifndef PLAYER_H
#define PLAYER_H

#include <string>
using namespace std;
#include "PiecePosition.h"

class Player {
public:
    Player(int color) : myColor(color) {};
    virtual ~Player() = default;
    int getColor() const { return myColor; }
protected:
    int myColor;
};

#endif
