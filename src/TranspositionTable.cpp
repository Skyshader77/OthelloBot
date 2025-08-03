#include "TranspositionTable.h"
#include <iostream>
using namespace std;

TranspositionTable::TranspositionTable(size_t sizeInMB){
        size = (sizeInMB * 1024 * 1024) / sizeof(TTEntry);
        // Make size a power of 2 for efficient indexing
        size_t powerOf2 = 1;
        while (powerOf2 < size) powerOf2 <<= 1;
        size = powerOf2;
        mask = size - 1;
        table.resize(size);
    }

void TranspositionTable::store(uint64_t hash, int depth, int value, piecePosition bestMove, 
        TTEntry::NodeType nodeType, bool hasBestMove) {
        size_t index = hash & mask;
        TTEntry& entry = table[index];
        
        if (entry.hash != hash || depth >= entry.depth) {
            entry.hash = hash;
            entry.depth = depth;
            entry.value = value;
            entry.bestMove = bestMove;
            entry.nodeType = nodeType;
            entry.hasBestMove = hasBestMove;
        }
    }   
    
bool TranspositionTable::probe(uint64_t hash, int depth, int alpha, int beta, TTEntry& entry) {
        size_t index = hash & mask;
        entry = table[index];
        
        if (entry.hash != hash || entry.depth < depth) {
            return false;
        }
        
        // Check if we can use this entry based on node type
        switch (entry.nodeType) {
            case TTEntry::EXACT:
                return true;
            case TTEntry::LOWER_BOUND:
                return entry.value >= beta;
            case TTEntry::UPPER_BOUND:
                return entry.value <= alpha;
        }
        return false;
    }

piecePosition TranspositionTable::getBestMove(uint64_t hash) {
        size_t index = hash & mask;
        TTEntry& entry = table[index];
        if (entry.hash == hash && entry.hasBestMove) {
            return entry.bestMove;
        }
        return piecePosition(); // Return invalid move if not found
    }

void TranspositionTable::clear() {
        fill(table.begin(), table.end(), TTEntry());
    }