#ifndef BITCOUNTER_H
#define BITCOUNTER_H

#include <cstdint>

class BitCounter
{
public:
    void initialize();
    int countSetBits(uint64_t n) const; 
private:
    int BitsSetTable256[256];
};

#endif