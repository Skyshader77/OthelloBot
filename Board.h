#include <cstdint>
#include <PiecePosition.h>

class Board
{
   uint64_t pieceBB[8];
public:
   enum enumPiece
   {
      nWhite,
      nBlack,  
   };

   uint64_t getWhitePieces() const {return  pieceBB[nWhite];}
   uint64_t getBlackPieces() const {return  pieceBB[nBlack];}
   uint64_t getAllPieces() const;
   bool isSquareOccupied(piecePosition newPiecePosition);
   bool isOutOfRange(piecePosition pos);
   bool isFull() const;
   uint64_t resetBoard();
   uint64_t updateBoard(piecePosition newPiecePosition);
   void printBoard() const;
};