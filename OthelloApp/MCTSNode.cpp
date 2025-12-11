#include "MCTSNode.h"
#include <algorithm>
#include "Constants.h"
#include <cmath>
#include "GameState.h"
#include <iostream>

// Strongly inspired from
// https://www.geeksforgeeks.org/machine-learning/monte-carlo-tree-search-mcts-in-machine-learning/
MCTSNode::MCTSNode() 
        : action_{0, 0, 0}, 
          numbVisits_(0),
          numbWins_(0),
          untriedActions_() 
    {
}

MCTSNode::~MCTSNode() {
    // Kill all children
    for(MCTSNode* child : children_) {
        delete child;  
    }
    children_.clear();
}

MCTSNode::MCTSNode(shared_ptr<Board> newState, MCTSNode* parentnode, piecePosition action, int playerColor)
    : state_(newState),
      previousNode_(parentnode),
      action_(action),
      playerColor_(playerColor),
      numbVisits_(0),
      numbWins_(0),
      untriedActions_()
{
}

void MCTSNode::setUntriedActions(const vector<piecePosition>& actions) {
    untriedActions_ = actions;
}

bool MCTSNode::isTerminalNode(){
    return state_->isFull();
}

bool MCTSNode::isFullyExpanded(){
    return untriedActions_.empty() && !children_.empty();
}

MCTSNode* MCTSNode::expand(){
    if(untriedActions_.empty()){
        return nullptr;
    }
    
    piecePosition actionToExpand = untriedActions_.back();
    untriedActions_.pop_back();
    
    actionToExpand.ncolor = playerColor_;
    
    shared_ptr<Board> newState = make_shared<Board>(*state_);
    newState->updateBoard(actionToExpand);
    
    int nextPlayerColor = (playerColor_ == enumPiece::nBlack) ? 
                          enumPiece::nWhite : enumPiece::nBlack;
    
    MCTSNode* childNode = new MCTSNode(newState, this, actionToExpand, nextPlayerColor);
    vector<piecePosition> childActions = newState->getEmptySpacesSortedByEdgeProximity();
    childNode->setUntriedActions(childActions);
    
    children_.push_back(childNode);
    
    return childNode;
}

float MCTSNode::computeUCBScore(MCTSNode* child){
    if(child->numbVisits_ == 0){
        return std::numeric_limits<float>::max();
    }
    
    float childWinRate = static_cast<float>(child->numbWins_) / child->numbVisits_;
    float parentWinRate = 1.0f - childWinRate;
    
    float exploreScore = EXPLORE_PARAMETER * sqrt(log(numbVisits_) / child->numbVisits_);
    
    return parentWinRate + exploreScore;
}

MCTSNode* MCTSNode::getBestChild(){
    if(children_.empty()){
        return nullptr;
    }
    
    for(const auto& child : children_){
        if(child->numbVisits_ == 0){
            return child;
        }
    }
    
    auto it = std::max_element(children_.begin(), children_.end(),
        [this](MCTSNode* a, MCTSNode* b){
            return computeUCBScore(a) < computeUCBScore(b);
        });
    
    return *it;
}

int MCTSNode::rollout(){
    GameState simulationState(state_, playerColor_);
    
    int nextPlayer = (playerColor_ == enumPiece::nBlack) ? 
                     enumPiece::nWhite : enumPiece::nBlack;
    
    while(simulationState.getCurrentPlayer() != nextPlayer) {
        simulationState.changeCurrentPlayer();
    }
    
    while(!simulationState.isGameOver()){
        vector<piecePosition> availableMoves = 
            simulationState.getBoard()->getEmptySpacesSortedByEdgeProximity();
        
        if(availableMoves.empty()){
            return DRAW_VALUE;
        }
        
        int randomIndex = rand() % availableMoves.size();
        piecePosition randomMove = availableMoves[randomIndex];
        randomMove.ncolor = simulationState.getCurrentPlayer();
        
        simulationState.getBoard()->updateBoard(randomMove);
        simulationState.changeCurrentPlayer();
    }
    
    return simulationState.getWinner();
}

void MCTSNode::backpropagate(int winner){
    numbVisits_++;
    
    if(winner == DRAW_VALUE){
        numbWins_ += 0.5;
    }
    else if(winner == playerColor_){
        numbWins_ += 1.0;
    }
    
    if(previousNode_ != nullptr){
        previousNode_->backpropagate(winner);
    }
}