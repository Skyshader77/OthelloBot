#include <cstdint>
#include <PiecePosition.h>

class BitCounter
{
public:
    void initialize();
    int countSetBits(uint64_t n) const; 
private:
    int BitsSetTable256[256];
};