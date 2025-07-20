#include "Board.h"
#include "Constants.h"
#include <iostream>
using namespace std;


Board::Board() {
    resetBoard();
}

uint64_t Board::resetBoard() {
    for(int i = 0; i < NUMB_COLORS; i++) {
        pieceBB[i] = 0ULL;
    }
    
    // Set initial Othello position
    // Center 4 squares: D4(White), E4(Black), D5(Black), E5(White)
    // Coordinates are 0-indexed: D4 = (3,3), E4 = (4,3), D5 = (3,4), E5 = (4,4)
    pieceBB[nWhite] = (1ULL << (3 * BOARD_SIZE + 3)) | (1ULL << (4 * BOARD_SIZE + 4)); // D4, E5
    pieceBB[nBlack] = (1ULL << (3 * BOARD_SIZE + 4)) | (1ULL << (4 * BOARD_SIZE + 3)); // E4, D5
    
    return pieceBB[nWhite] | pieceBB[nBlack];
}

uint64_t Board::getAllPieces() const {
    return pieceBB[nWhite] | pieceBB[nBlack];
}

bool Board::isOutOfRange(piecePosition pos) {
   if(pos.xCoord < 0 || pos.xCoord > (BOARD_SIZE-1) || 
       pos.yCoord < 0 || pos.yCoord > (BOARD_SIZE-1)) {
        return true;
    }
}

bool Board::isSquareOccupied(piecePosition pos) {
    if (isOutOfRange(pos)) {
        throw out_of_range("Position coordinates are outside the valid board range (0-7)");
    }
    
    int square = pos.yCoord * BOARD_SIZE + pos.xCoord;
    uint64_t squareMask = 1ULL << square;
    
    return (pieceBB[nWhite] | pieceBB[nBlack]) & squareMask;
}

bool Board::isFull() const {
    return (pieceBB[nWhite] | pieceBB[nBlack]) == FULL_BOARD;
}

void Board::printBoard() const {
    for(int y = 0; y < BOARD_SIZE; y++) {
        for(int x = 0; x < BOARD_SIZE; x++) {
            int square = y * BOARD_SIZE + x;
            uint64_t mask = 1ULL << square;
            
            if(pieceBB[nWhite] & mask) {
                cout << WHITE;
            } else if(pieceBB[nBlack] & mask) {
                cout << BLACK;
            } else {
                cout << EMPTY;
            }
        }
        cout << endl;
    }
}