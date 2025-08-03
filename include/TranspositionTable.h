
#include <PiecePosition.h>
#include <TTentry.h>
#include <vector>

class TranspositionTable {
private:
    vector<TTEntry> table;
    size_t size;
    size_t mask;
    
public:
    TranspositionTable(size_t sizeInMB = 64);
    void store(uint64_t hash, int depth, int value, piecePosition bestMove, 
               TTEntry::NodeType nodeType, bool hasBestMove);
    bool probe(uint64_t hash, int depth, int alpha, int beta, TTEntry& entry); 
    piecePosition getBestMove(uint64_t hash);
    void clear();
};