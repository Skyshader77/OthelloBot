#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <PiecePosition.h>
#include <Constants.h>
#include <vector>
class Board
{
public:
   Board();
   uint64_t getBoard() const;
   uint64_t getWhitePieces() const {return  pieceBB[nWhite];}
   uint64_t getBlackPieces() const {return  pieceBB[nBlack];}
   uint64_t getAllPieces() const;
   vector<piecePosition> getEmptySpaces() const;
   bool isInValidPosition(piecePosition pos);
   bool isSquareOccupied(piecePosition newPiecePosition);
   bool isOutOfRange(piecePosition pos);
   bool hasAlly(piecePosition pos, int ncolor);
   bool isFull() const;
   uint64_t resetBoard();
   void updateBoard(piecePosition newPiecePosition);
   void printBoard() const;
private:
   uint64_t pieceBB[8];
};

#endif