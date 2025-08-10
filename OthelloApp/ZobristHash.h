#ifndef ZOBRISTHASH_H
#define ZOBRISTHASH_H

#include "Constants.h"
#include "GameState.h"
#include "PiecePosition.h"

class ZobristHash {
private:
    static const int BOARD_SIZE = 64; // Assuming 8x8 board
    static const int NUM_PIECE_TYPES = 2;
    uint64_t pieceKeys[BOARD_SIZE][NUM_PIECE_TYPES];
    uint64_t sideToMoveKey;
    bool initialized;
    
    // Simple random number generator for consistent hash keys
    uint64_t rng_state;
    
    uint64_t xorshift64() {
        rng_state ^= rng_state << 13;
        rng_state ^= rng_state >> 7;
        rng_state ^= rng_state << 17;
        return rng_state;
    }
public:
    ZobristHash();
    void initializeKeys();
    uint64_t computeHash(GameState* gameState);
};

#endif
