#include "Board.h"
#include "Constants.h"
#include <iostream>
using namespace std;
#include <iostream>
#include <algorithm>
#include <set>

Board::Board() {

}

Board::Board(const Board& board){
    for(int i = 0; i < 8; i++){
        pieceBB[i] = board.pieceBB[i];
    }
}
uint64_t Board::resetBoard() {
    for(int i = 0; i < NUMB_COLORS; i++) {
        pieceBB[i] = 0ULL;
    }
    
    // Set initial Othello position
    // Center 4 squares: D4(White), E4(Black), D5(Black), E5(White)
    // Coordinates are 0-indexed: D4 = (3,3), E4 = (4,3), D5 = (3,4), E5 = (4,4)
    pieceBB[nWhite] = (1ULL << (3 * BOARD_LENGTH + 3)) | (1ULL << (4 * BOARD_LENGTH + 4)); // D4, E5
    pieceBB[nBlack] = (1ULL << (3 * BOARD_LENGTH + 4)) | (1ULL << (4 * BOARD_LENGTH + 3)); // E4, D5
    
    return pieceBB[nWhite] | pieceBB[nBlack];
}

uint64_t Board::getAllPieces() const {
    return pieceBB[nWhite] | pieceBB[nBlack];
}

bool Board::isOutOfRange(piecePosition pos) {
    if(pos.xCoord < 0 || pos.xCoord >= BOARD_LENGTH ||
       pos.yCoord < 0 || pos.yCoord >= BOARD_LENGTH) {
        return true;
    }
    return false;
}

vector<piecePosition> Board::getEmptySpaces() const {
    vector<piecePosition> emptySpaces;
    uint64_t occupiedSquares = getAllPieces();
    
    for(int y = 0; y < BOARD_LENGTH; y++) {
        for(int x = 0; x < BOARD_LENGTH; x++) {
            int square = y * BOARD_LENGTH + x;
            uint64_t squareMask = 1ULL << square;
            
            if(!(occupiedSquares & squareMask)) {
                piecePosition emptyPos;
                emptyPos.xCoord = x;
                emptyPos.yCoord = y;
                emptyPos.ncolor = -1; // No color for empty spaces
                emptySpaces.push_back(emptyPos);
            }
        }
    }
    
    return emptySpaces;
}

vector<piecePosition> Board::getEmptySpacesSortedByEdgeProximity() const {
    vector<piecePosition> emptySpaces = getEmptySpaces();
    
    sort(emptySpaces.begin(), emptySpaces.end(), [](const piecePosition& a, const piecePosition& b) {
        int distA = min({a.xCoord, a.yCoord, BOARD_LENGTH - 1 - a.xCoord, BOARD_LENGTH - 1 - a.yCoord});
        
        int distB = min({b.xCoord, b.yCoord, BOARD_LENGTH - 1 - b.xCoord, BOARD_LENGTH - 1 - b.yCoord});
        
        if (distA == distB) {
            if (a.yCoord == b.yCoord) {
                return a.xCoord < b.xCoord;
            }
            return a.yCoord < b.yCoord;
        }
        
        return distA < distB;
    });
    
    return emptySpaces;
}

vector<piecePosition> Board::getCornerEmptySpaces() const {
    vector<piecePosition> cornerSpaces;
    uint64_t occupiedSquares = getAllPieces();
    
    for (const auto& corner : CORNERS) {
        int x = corner.first;
        int y = corner.second;
        int square = y * BOARD_LENGTH + x;
        uint64_t squareMask = 1ULL << square;
        
        if (!(occupiedSquares & squareMask)) {
            piecePosition emptyCorner;
            emptyCorner.xCoord = x;
            emptyCorner.yCoord = y;
            emptyCorner.ncolor = -1; 
            cornerSpaces.push_back(emptyCorner);
        }
    }
    
    return cornerSpaces;
}

int Board::getPieceColorAt(int x, int y) const {    
    int square = y * BOARD_LENGTH + x;
    uint64_t squareMask = 1ULL << square;
    
    if (getBlackPieces() & squareMask) {
        return  nBlack;
    } else if (getWhitePieces() & squareMask) {
        return  nWhite;
    }
    
    return -1;
}

std::vector<piecePosition> Board::getEmptyTilesOrthogonalToSameColorCorners(int playerColor) const {
    std::vector<piecePosition> adjacentEmptyTiles;
    std::set<std::pair<int, int>> addedPositions;
    uint64_t occupiedSquares = getAllPieces();
    
    for (const auto& [cornerX, cornerY] : CORNERS) {
        int cornerSquare = cornerY * BOARD_LENGTH + cornerX;
        uint64_t cornerMask = 1ULL << cornerSquare;
        
        if (!(occupiedSquares & cornerMask) || 
            getPieceColorAt(cornerX, cornerY) != playerColor) {
            continue;
        }
        
        for (const auto& [dx, dy] : ORTHOGONAL_OFFSETS) {
            int adjX = cornerX + dx;
            int adjY = cornerY + dy;
            
            if (adjX >= 0 && adjX < BOARD_LENGTH && 
                adjY >= 0 && adjY < BOARD_LENGTH) {
                
                int adjSquare = adjY * BOARD_LENGTH + adjX;
                uint64_t adjMask = 1ULL << adjSquare;
                
                if (!(occupiedSquares & adjMask) && 
                    addedPositions.insert({adjX, adjY}).second) {
                    
                    adjacentEmptyTiles.push_back({adjX, adjY, playerColor});
                }
            }
        }
    }
    
    return adjacentEmptyTiles;
}

void Board::updateBoard(piecePosition newPiecePosition) {
    if (isInValidPosition(newPiecePosition)) {
        return;
    }
    
    if (newPiecePosition.ncolor < 0 || newPiecePosition.ncolor > 1) {
        cout << "ERROR: Invalid color " << newPiecePosition.ncolor << endl;
        return;
    }
    
    int square = newPiecePosition.yCoord * BOARD_LENGTH + newPiecePosition.xCoord;
    if (square < 0 || square >= 64) {  // assuming 8x8 board
        cout << "ERROR: Invalid square " << square << endl;
        return;
    }
    
    pieceBB[newPiecePosition.ncolor] |= (1ULL << square);

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
        for (const pair<int, int>& piece : capturedPieces) {
            int captureSquare = piece.second * BOARD_LENGTH + piece.first;
            if (captureSquare >= 0 && captureSquare < 64) {
                uint64_t pieceMask = 1ULL << captureSquare;
                pieceBB[1 - newPiecePosition.ncolor] &= ~pieceMask;
                pieceBB[newPiecePosition.ncolor] |= pieceMask;
            }
        }
    }
}

bool Board::isInValidPosition(piecePosition pos){
    return isOutOfRange(pos) || isSquareOccupied(pos);
}

bool Board::isSquareOccupied(piecePosition pos) {
    if (isOutOfRange(pos)){
        return false;
    }
    int square = pos.yCoord * BOARD_LENGTH + pos.xCoord;
    uint64_t squareMask = 1ULL << square;
    
    return (pieceBB[nWhite] | pieceBB[nBlack]) & squareMask;
}

bool Board::hasAlly(piecePosition pos, int ncolor) {
    if (isOutOfRange(pos)){
        return false;
    }
    int square = pos.yCoord * BOARD_LENGTH + pos.xCoord;
    uint64_t squareMask = 1ULL << square;
    
    return (pieceBB[ncolor]) & squareMask;
}

bool Board::isFull() const {
    return (pieceBB[nWhite] | pieceBB[nBlack]) == FULL_BOARD;
}

void Board::printBoard() const {
    cout << "\n    ";
    for(int x = 0; x < BOARD_LENGTH; x++) {
        cout << "  " << char('A' + x) << " ";
    }
    cout << "\n";
    
    cout << "    ┌";
    for(int x = 0; x < BOARD_LENGTH; x++) {
        cout << "───";
        if(x < BOARD_LENGTH - 1) cout << "┬";
    }
    cout << "┐\n";
    
    for(int y = 0; y < BOARD_LENGTH; y++) {
        cout << " " << (y + 1) << "  │";
        
        for(int x = 0; x < BOARD_LENGTH; x++) {
            int square = y * BOARD_LENGTH + x;
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
        
        if(y < BOARD_LENGTH - 1) {
            cout << "    ├";
            for(int x = 0; x < BOARD_LENGTH; x++) {
                cout << "───";
                if(x < BOARD_LENGTH - 1) cout << "┼";
            }
            cout << "┤\n";
        }
    }
    
    cout << "    └";
    for(int x = 0; x < BOARD_LENGTH; x++) {
        cout << "───";
        if(x < BOARD_LENGTH - 1) cout << "┴";
    }
    cout << "┘\n";
    
    cout << "    ";
    for(int x = 0; x < BOARD_LENGTH; x++) {
        cout << "  " << char('A' + x) << " ";
    }
    cout << "\n\n";
    
    cout << "Legend: ● White  ○ Black  · Empty\n";
}