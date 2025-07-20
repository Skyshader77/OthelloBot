#include "Board.h"
#include "Constants.h"
#include <iostream>
using namespace std;
#include <iostream>
#include <vector>

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
    return false;
}

void Board::updateBoard(piecePosition newPiecePosition) {
    if (isInValidPosition(newPiecePosition)) {
        return; 
    }

    pieceBB[newPiecePosition.ncolor] |= (1ULL << (newPiecePosition.yCoord * BOARD_SIZE + newPiecePosition.xCoord)); 

    vector<pair<int, int>> capturedPieces;
    
    for (int(&direction)[2] : DIRECTIONS) {
        vector<pair<int, int>> lookedPieces;
        piecePosition searchedPosition = newPiecePosition;
        
        searchedPosition.xCoord += direction[0];
        searchedPosition.yCoord += direction[1];
        
        while (!isOutOfRange(searchedPosition) && 
               isSquareOccupied(searchedPosition) && 
               !hasAlly(searchedPosition, newPiecePosition.ncolor)) {
            
            lookedPieces.push_back(make_pair(searchedPosition.xCoord, searchedPosition.yCoord));
            searchedPosition.xCoord += direction[0];
            searchedPosition.yCoord += direction[1];
        }
        
        if (!isOutOfRange(searchedPosition) && 
            hasAlly(searchedPosition, newPiecePosition.ncolor) && 
            !lookedPieces.empty()) {
            capturedPieces.insert(capturedPieces.end(), lookedPieces.begin(), lookedPieces.end());
        }
    }
    
    if (!capturedPieces.empty()) {
        int newSquare = newPiecePosition.yCoord * BOARD_SIZE + newPiecePosition.xCoord;
        pieceBB[newPiecePosition.ncolor] |= (1ULL << newSquare);
        
        for (const pair<int, int>& piece : capturedPieces) {
            int square = piece.second * BOARD_SIZE + piece.first;
            uint64_t pieceMask = 1ULL << square;
            
            pieceBB[1 - newPiecePosition.ncolor] &= ~pieceMask;
            pieceBB[newPiecePosition.ncolor] |= pieceMask;
        }
        
    }

    cout << "Piece placed at (" << newPiecePosition.xCoord << ", " << newPiecePosition.yCoord 
         << ") with color " << newPiecePosition.ncolor << endl;
    
}

bool Board::isInValidPosition(piecePosition pos){
    return isOutOfRange(pos) || isSquareOccupied(pos);
}

bool Board::isSquareOccupied(piecePosition pos) {
    int square = pos.yCoord * BOARD_SIZE + pos.xCoord;
    uint64_t squareMask = 1ULL << square;
    
    return (pieceBB[nWhite] | pieceBB[nBlack]) & squareMask;
}

bool Board::hasAlly(piecePosition pos, int ncolor) {
    int square = pos.yCoord * BOARD_SIZE + pos.xCoord;
    uint64_t squareMask = 1ULL << square;
    
    return (pieceBB[ncolor]) & squareMask;
}

bool Board::isFull() const {
    return (pieceBB[nWhite] | pieceBB[nBlack]) == FULL_BOARD;
}

void Board::printBoard() const {
    cout << "\n    ";
    for(int x = 0; x < BOARD_SIZE; x++) {
        cout << "  " << char('A' + x) << " ";
    }
    cout << "\n";
    
    cout << "    ┌";
    for(int x = 0; x < BOARD_SIZE; x++) {
        cout << "───";
        if(x < BOARD_SIZE - 1) cout << "┬";
    }
    cout << "┐\n";
    
    for(int y = 0; y < BOARD_SIZE; y++) {
        cout << " " << (y + 1) << "  │";
        
        for(int x = 0; x < BOARD_SIZE; x++) {
            int square = y * BOARD_SIZE + x;
            uint64_t mask = 1ULL << square;
            
            cout << " ";
            if(pieceBB[nWhite] & mask) {
                cout << "●";
            } else if(pieceBB[nBlack] & mask) {
                cout << "○";
            } else {
                cout << "·";
            }
            cout << " │";
        }
        cout << " " << (y + 1) << "\n";
        
        if(y < BOARD_SIZE - 1) {
            cout << "    ├";
            for(int x = 0; x < BOARD_SIZE; x++) {
                cout << "───";
                if(x < BOARD_SIZE - 1) cout << "┼";
            }
            cout << "┤\n";
        }
    }
    
    cout << "    └";
    for(int x = 0; x < BOARD_SIZE; x++) {
        cout << "───";
        if(x < BOARD_SIZE - 1) cout << "┴";
    }
    cout << "┘\n";
    
    cout << "    ";
    for(int x = 0; x < BOARD_SIZE; x++) {
        cout << "  " << char('A' + x) << " ";
    }
    cout << "\n\n";
    
    cout << "Legend: ● White  ○ Black  · Empty\n";
}