#include <string>
using namespace std;
#include <PiecePosition.h>

class Player {
public:
    Player(int color) : myColor(color) {};
    piecePosition pickAction();
    piecePosition insertPiece(); 
    void leaveGame();
private:
    int myColor;
};