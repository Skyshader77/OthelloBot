#ifndef MCTS_NODE_H
#define MCST_NODE_H

#include "Board.h"
#include "Player.h"
#include "PiecePosition.h"
#include <vector>
#include "GameState.h"

class MCTSNode
{
public:
    MCTSNode();
    ~MCTSNode();
    MCTSNode(shared_ptr<Board> newState, MCTSNode* parentnode, piecePosition action, int playerColor);
    bool isTerminalNode();
    bool isFullyExpanded();
    MCTSNode* expand();
    MCTSNode* getBestChild();
    float computeUCBScore(MCTSNode* child);
    int rollout();
    void backpropagate(int winner);
    void setUntriedActions(const vector<piecePosition>& actions);
    shared_ptr<Board> getState() const { return state_; }
    piecePosition getAction() const { return action_; }
    int getVisits() const { return numbVisits_; }
    float getWins() const { return numbWins_; }
    vector<MCTSNode*> getChildren() const { return children_; }
private:
    shared_ptr<Board> state_;
    int playerColor_;
    MCTSNode* previousNode_;
    vector<MCTSNode*> children_;
    piecePosition action_;
    int numbVisits_;
    int numbWins_;
    vector<piecePosition> untriedActions_;
};
#endif