#include "ZobristHash.h"

ZobristHash::ZobristHash() : initialized(false), rng_state(0x123456789ABCDEFULL) {
        initializeKeys();
    }

void ZobristHash::initializeKeys() {
        if (initialized) return;
        
        // fill a table of random numbers/bitstrings
        for (int square = 0; square < BOARD_SIZE; ++square) {
            for (int pieceType = 0; pieceType < NUM_PIECE_TYPES; ++pieceType) {
                pieceKeys[square][pieceType] = xorshift64();
            }
        }
        
        // Initialize side to move key
        sideToMoveKey = xorshift64();
        
        initialized = true;
    }

uint64_t ZobristHash::computeHash(GameState* gameState) {
        uint64_t hash = 0;
        auto board = gameState->getBoard();
        
        uint64_t whitePieces = board->getWhitePieces();
        uint64_t blackPieces = board->getBlackPieces();
        
        // Hash white pieces
        for (int square = 0; square < BOARD_SIZE; ++square) {
            if (whitePieces & (1ULL << square)) {
                hash ^= pieceKeys[square][nWhite]; // 0 for white
            }
        }
        
        // Hash black pieces
        for (int square = 0; square < BOARD_SIZE; ++square) {
            if (blackPieces & (1ULL << square)) {
                hash ^= pieceKeys[square][nBlack]; // 1 for black
            }
        }
        
        // Hash side to move
        if (gameState->getCurrentPlayer() == nWhite) {
            hash ^= sideToMoveKey;
        }
        
        return hash;
    }