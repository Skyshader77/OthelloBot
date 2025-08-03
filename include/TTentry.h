#ifndef TTENTRY_H
#define TTENTRY_H

using namespace std;
#include <PiecePosition.h>
#include <cstdint>

struct TTEntry {
    uint64_t hash;         
    int depth;              
    int value;               
    piecePosition bestMove;  
    enum NodeType { EXACT, LOWER_BOUND, UPPER_BOUND } nodeType;
    bool hasBestMove;
    
    TTEntry() : hash(0), depth(-1), value(0), nodeType(EXACT), hasBestMove(false) {}
};

#endif