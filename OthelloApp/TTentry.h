#ifndef TTENTRY_H
#define TTENTRY_H

using namespace std;
#include "PiecePosition.h"
#include <cstdint>

struct TTEntry {
    uint64_t hash;         
    int depth;              
    int value;               
    piecePosition bestMove;  
    bool hasBestMove;
    
    TTEntry() : hash(0), depth(-1), value(0), hasBestMove(false) {}
};

#endif
