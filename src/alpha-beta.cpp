// C++ program to demonstrate
// working of Alpha-Beta Pruning
// https://www.geeksforgeeks.org/dsa/minimax-algorithm-in-game-theory-set-4-alpha-beta-pruning/
#include<bits/stdc++.h>
using namespace std;
#include<GameState.h>
#include<Alpha-beta.h>
#include<ZobristHash.h>
#include<TTentry.h>
#include<TranspositionTable.h>

TranspositionTable tt(64);
ZobristHash zobrist;

uint64_t computePositionHash(GameState* gameState) {
    return zobrist.computeHash(gameState);
}

int compute_heuristic(GameState* gameState, int originalColor){
    int numbWhitePieces=0;
    int numbBlackPieces=0;
    int heuristicScore=0;
    numbWhitePieces=gameState->getNumbWhitePieces();
    numbBlackPieces=gameState->getNumbBlackPieces();

    if (originalColor==nWhite){
        heuristicScore=numbWhitePieces-numbBlackPieces;
    } else{
        heuristicScore=numbBlackPieces-numbWhitePieces;
    }
    return heuristicScore;
}


MinimaxResult minimax(int depth, bool maximizingPlayer, int alpha, int beta, 
                     GameState* gamestate, int originalColor, uint64_t hash) {
    
    // Check transposition table first
    TTEntry ttEntry;
    if (tt.probe(hash, depth, alpha, beta, ttEntry)) {
        MinimaxResult result;
        result.value = ttEntry.value;
        result.bestMove = ttEntry.bestMove;
        result.hasBestMove = ttEntry.hasBestMove;
        return result;
    }
    
    if (depth == 0 || gamestate->isGameOver()) {
        MinimaxResult result;
        result.value = compute_heuristic(gamestate, originalColor);
        result.hasBestMove = false;
        
        tt.store(hash, depth, result.value, piecePosition(), false);
        return result;
    }
    
    vector<piecePosition> listPossibleMoves = gamestate->getBoard()->getEmptySpaces();
    
    // Move ordering: try the best move from transposition table first
    piecePosition ttBestMove = tt.getBestMove(hash);
    if (ttBestMove.xCoord != 0 || ttBestMove.yCoord != 0) { // Valid move check
        // Move the TT best move to the front
        auto it = find_if(listPossibleMoves.begin(), listPossibleMoves.end(),
            [&ttBestMove](const piecePosition& move) {
                return move.xCoord == ttBestMove.xCoord && move.yCoord == ttBestMove.yCoord;
            });
        if (it != listPossibleMoves.end()) {
            swap(*listPossibleMoves.begin(), *it);
        }
    }
    
    MinimaxResult finalResult;
    TTEntry::NodeType nodeType = TTEntry::UPPER_BOUND;
    
    if (maximizingPlayer) {
        int best = MIN;
        piecePosition bestMove;
        bool foundMove = false;
        
        for (auto move = listPossibleMoves.begin(); move != listPossibleMoves.end(); ++move) {
            piecePosition newmove = *move;
            newmove.ncolor = gamestate->getCurrentPlayer();
            
            GameState newGameState = gamestate->deepCopy();
            newGameState.changeCurrentPlayer();
            newGameState.getBoard()->updateBoard(newmove);
            
            uint64_t newHash = zobrist.computeHash(&newGameState);
            
            MinimaxResult result = minimax(depth - 1, false, alpha, beta, 
                                         &newGameState, originalColor, newHash);
            
            if (result.value > best) {
                best = result.value;
                bestMove = newmove;
                foundMove = true;
            }
            
            alpha = max(alpha, best);
            
            if (beta <= alpha) {
                break;
            }
        }
        
        finalResult.value = best;
        finalResult.bestMove = bestMove;
        finalResult.hasBestMove = foundMove;
        
    } else {
        int best = MAX;
        piecePosition bestMove;
        bool foundMove = false;
        
        for (auto move = listPossibleMoves.begin(); move != listPossibleMoves.end(); ++move) {
            piecePosition newmove = *move;
            newmove.ncolor = gamestate->getCurrentPlayer();
            
            GameState newGameState = gamestate->deepCopy();
            newGameState.changeCurrentPlayer();
            newGameState.getBoard()->updateBoard(newmove);
            
            uint64_t newHash = zobrist.computeHash(&newGameState);
            
            MinimaxResult result = minimax(depth - 1, true, alpha, beta, 
                                         &newGameState, originalColor, newHash);
            
            if (result.value < best) {
                best = result.value;
                bestMove = newmove;
                foundMove = true;
            }
            
            beta = min(beta, best);
            
            if (beta <= alpha) {
                break;
            }
        }
        
        
        finalResult.value = best;
        finalResult.bestMove = bestMove;
        finalResult.hasBestMove = foundMove;
    }
    
    tt.store(hash, depth, finalResult.value, finalResult.bestMove, finalResult.hasBestMove);
    
    return finalResult;
}

MinimaxResult iterativeDeepeningSearch(GameState* gamestate, int maxDepth, 
                                     int originalColor, int timeLimit) {
    MinimaxResult bestResult;
    bestResult.value = (originalColor == gamestate->getCurrentPlayer()) ? MIN : MAX;
    bestResult.hasBestMove = false;
    
    auto startTime = chrono::high_resolution_clock::now();
    
    // Clear transposition table for new search
    // tt.clear(); // Uncomment if you want fresh search each time
    
    for (int depth = 1; depth <= maxDepth; ++depth) {
        if (timeLimit > 0) {
            auto currentTime = chrono::high_resolution_clock::now();
            auto elapsed = chrono::duration_cast<chrono::milliseconds>
                          (currentTime - startTime).count();
            if (elapsed >= timeLimit) {
                cout << "Time limit reached at depth " << depth - 1 << endl;
                break;
            }
        }
        
        cout << "Searching depth " << depth << "..." << endl;
        
        uint64_t hash = computePositionHash(gamestate);
        bool isMaximizing = (originalColor == gamestate->getCurrentPlayer());
        
        MinimaxResult result = minimax(depth, isMaximizing, MIN, MAX, 
                                     gamestate, originalColor, hash);
        
        // Update best result
        if (result.hasBestMove) {
            bestResult = result;
            cout << "Depth " << depth << " completed. Best value: " 
                 << result.value << endl;
        }
        
        // If we found a winning/losing position, no need to search deeper
        if (abs(result.value) > MAX) {
            cout << "Terminal position found at depth " << depth << endl;
            break;
        }
    }
    
    auto endTime = chrono::high_resolution_clock::now();
    auto totalTime = chrono::duration_cast<chrono::milliseconds>
                    (endTime - startTime).count();
    cout << "Search completed in " << totalTime << " ms" << endl;
    
    return bestResult;
}

MinimaxResult minimaxSimple(int depth, bool maximizingPlayer, int alpha, int beta, GameState* gamestate, int originalColor) {
    if (depth == MAX_DEPTH || gamestate->isGameOver()) {
        MinimaxResult result;
        result.value = compute_heuristic(gamestate, originalColor);
        result.hasBestMove = false;
        return result;
    }
    
    vector<piecePosition> listPossibleMoves = gamestate->getBoard()->getEmptySpaces();
    if (maximizingPlayer) {
        int best = MIN;
        piecePosition bestMove;
        bool foundMove = false;
        
        for (auto move = listPossibleMoves.begin(); move != listPossibleMoves.end(); ++move) {
            piecePosition newmove = *move;
            newmove.ncolor = gamestate->getCurrentPlayer();
            GameState newGameState = gamestate->deepCopy();
            newGameState.changeCurrentPlayer();
            newGameState.getBoard()->updateBoard(newmove);
            
            MinimaxResult result = minimaxSimple(depth + 1, false, alpha, beta, &newGameState, originalColor);
            
            if (result.value > best) {
                best = result.value;
                bestMove = newmove;
                foundMove = true;
            }
            
            alpha = max(alpha, best);
            
            // Alpha Beta Pruning
            if (beta <= alpha)
                break;
        }
        
        MinimaxResult finalResult;
        finalResult.value = best;
        finalResult.bestMove = bestMove;
        finalResult.hasBestMove = foundMove;
        return finalResult;
    }
    else {
        int best = MAX;
        piecePosition bestMove;
        bool foundMove = false;
        
        for (auto move = listPossibleMoves.begin(); move != listPossibleMoves.end(); ++move) {
            piecePosition newmove = *move;
            newmove.ncolor = gamestate->getCurrentPlayer();
            GameState newGameState = gamestate->deepCopy();
            newGameState.changeCurrentPlayer();
            newGameState.getBoard()->updateBoard(newmove);
            
            MinimaxResult result = minimaxSimple(depth + 1, true, alpha, beta, &newGameState, originalColor);
            
            if (result.value < best) {
                best = result.value;
                bestMove = newmove;
                foundMove = true;
            }
            
            beta = min(beta, best);
            
            if (beta <= alpha)
                break;
        }
        
        MinimaxResult finalResult;
        finalResult.value = best;
        finalResult.bestMove = bestMove;
        finalResult.hasBestMove = foundMove;

        return finalResult;
    }
}