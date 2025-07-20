#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <PiecePosition.h>

class Board
{
public:
   enum enumPiece
   {
      nWhite,
      nBlack,  
   };
   Board();
   uint64_t getBoard() const;
   uint64_t getWhitePieces() const {return  pieceBB[nWhite];}
   uint64_t getBlackPieces() const {return  pieceBB[nBlack];}
   uint64_t getAllPieces() const;
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