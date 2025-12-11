
#include"MCTSAlgo.h"
#include"MCTSNode.h"
#include <algorithm>
#include <chrono>
#include <iostream>


BestAction mctsSearchTimed(GameState* gameState, int maxIterations, int timeLimit) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    MCTSNode* root = new MCTSNode(
        gameState->getBoard(), 
        nullptr, 
        piecePosition{0, 0, 0}, 
        gameState->getCurrentPlayer()
    );
    root->setUntriedActions(gameState->getBoard()->getEmptySpacesSortedByEdgeProximity());
    
    int iterationsCompleted = 0;
    
    for(int i = 0; i < maxIterations; i++) {
        if(timeLimit > 0) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
                          (currentTime - startTime).count();
            if(elapsed >= timeLimit) {
                cout << "Time limit reached at iteration " << i << endl;
                break;
            }
        }
        
        MCTSNode* node = root;
        
        while(!node->isTerminalNode() && node->isFullyExpanded()) {
            node = node->getBestChild();
            if(node == nullptr) {
                break;
            }
        }
        
        if(node == nullptr) {
            continue;
        }
        
        if(!node->isTerminalNode() && !node->isFullyExpanded()) {
            MCTSNode* expandedNode = node->expand();
            if(expandedNode != nullptr) {
                node = expandedNode;
            }
        }
        
        int winner = node->rollout();
        
        node->backpropagate(winner);
        
        iterationsCompleted++;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>
                    (endTime - startTime).count();
    
    cout << "MCTS completed " << iterationsCompleted << " iterations in " 
         << totalTime << " ms" << endl;
    
    BestAction result;
    result.hasAction = false;
    result.iterationsCompleted = iterationsCompleted;

    if(root->getChildren().empty()) {
        delete root;
        return result;
    }

    MCTSNode* bestChild = nullptr;
    int maxVisits = -1;

    for(MCTSNode* child : root->getChildren()) {
        if(child == nullptr) {
            continue;
        }
        
        int visits = child->getVisits();
        if(visits > maxVisits) {
            maxVisits = visits;
            bestChild = child;
        }
    }

    if(bestChild == nullptr) {
        cout << "ERROR: No valid best child found!" << endl;
        result.hasAction = false;
        delete root;
        return result;
    }

    result.action = bestChild->getAction();
    result.visits = bestChild->getVisits();

    float childWinRate = static_cast<float>(bestChild->getWins()) / bestChild->getVisits();
    result.winRate = 1.0f - childWinRate; 

    result.hasAction = true;

    cout << "Best move: (" << result.action.xCoord << ", " << result.action.yCoord 
        << ") with " << result.visits << " visits and " 
        << (result.winRate * 100.0f) << "% win rate for player " << gameState->getCurrentPlayer() << endl;
    
    cout << "Starting cleanup of MCTS tree..." << endl;
    delete root;
    cout << "Cleanup completed" << endl;

    return result;
}