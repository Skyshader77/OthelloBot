
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
        // Check time limit
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
        
        // Selection: traverse tree using UCB until we find a node that's not fully expanded
        while(!node->isTerminalNode() && node->isFullyExpanded()) {
            node = node->getBestChild();
            if(node == nullptr) {
                break;
            }
        }
        
        if(node == nullptr) {
            continue;
        }
        
        // Expansion: if node is not terminal and not fully expanded, expand it
        if(!node->isTerminalNode() && !node->isFullyExpanded()) {
            MCTSNode* expandedNode = node->expand();
            if(expandedNode != nullptr) {
                node = expandedNode;
            }
        }
        
        // Simulation (Rollout): simulate random game from current node
        GameState simulationState=gameState->deepCopy();
        int winner = node->rollout(&simulationState);
        
        // Backpropagation: update statistics back up the tree
        node->backpropagate(winner);
        
        iterationsCompleted++;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>
                    (endTime - startTime).count();
    
    cout << "MCTS completed " << iterationsCompleted << " iterations in " 
         << totalTime << " ms" << endl;
    
    // Select best child based on visit count (most robust)
    BestAction result;
    result.hasAction = false;
    result.iterationsCompleted = iterationsCompleted;

    if(root->getChildren().empty()) {
        delete root;
        return result;
    }

    // First, let's check each child manually
    for(size_t i = 0; i < root->getChildren().size(); i++) {
        MCTSNode* child = root->getChildren()[i];
        if(child == nullptr) {
            cout << "Child " << i << " is NULL!" << endl;
        } else {
            cout << "Child " << i << ": visits=" << child->getVisits() 
                << ", wins=" << child->getWins() 
                << ", action=(" << child->getAction().xCoord << "," << child->getAction().yCoord << ")" << endl;
        }
    }

    cout.flush();  // Force output
    // Use simpler manual selection instead
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

    result.action = bestChild->getAction();
    result.visits = bestChild->getVisits();
    result.winRate = static_cast<float>(bestChild->getWins()) / bestChild->getVisits();
    result.hasAction = true;
    
    cout << "Best move: (" << result.action.xCoord << ", " << result.action.yCoord 
         << ") with " << result.visits << " visits and " 
         << (result.winRate * 100.0f) << "% win rate" << endl;
    
    // Cleanup
    cout << "Starting cleanup..." << endl;
    delete root;
    cout << "Cleanup completed" << endl;

    return result;
}