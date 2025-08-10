#include "BitCounter.h"

// Function to initialise the lookup table 
void BitCounter::initialize() 
{ 
    BitsSetTable256[0] = 0; 
    for (int i = 0; i < 256; i++)
    { 
        BitsSetTable256[i] = (i & 1) + 
        BitsSetTable256[i / 2]; 
    } 
} 


int BitCounter::countSetBits(uint64_t n) const
{ 
    return (BitsSetTable256[n & 0xff] + 
            BitsSetTable256[(n >> 8) & 0xff] + 
            BitsSetTable256[(n >> 16) & 0xff] + 
            BitsSetTable256[(n >> 24) & 0xff] +
            BitsSetTable256[(n >> 32) & 0xff] +
            BitsSetTable256[(n >> 40) & 0xff] +
            BitsSetTable256[(n >> 48) & 0xff] +
            BitsSetTable256[(n >> 56) & 0xff]); 
} 
