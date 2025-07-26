#include "GameState.h"
#include <iostream>
using namespace std;
#include <iostream>
#include <vector>
#include<BotPlayer.h>
#include<HumanPlayer.h>
#include "Constants.h"

GameState::GameState() : currentPlayer(0) {
    board = make_shared<Board>();
    bitcounter.initialize();
}

shared_ptr<Board> GameState::getBoard() const{
    return board;
}

bool GameState::isGameOver() const{
    return board->isFull();
}

int GameState::getCurrentPlayer() const{
    return currentPlayer;
}

void GameState::changeCurrentPlayer() {
    currentPlayer=1-currentPlayer;
}

void GameState::addComputerPlayer(){
    int newColor=nBlack;
    if (!players.empty()) {
        newColor=nWhite;
    } 
    players.push_back(make_shared<BotPlayer>(newColor));
}

void GameState::addHumanPlayer(){
    int newColor=nBlack;
    if (!players.empty()) {
        newColor=nWhite;
    } 
    players.push_back(make_shared<HumanPlayer>(newColor));
}

GameState GameState::deepCopy() const {
    GameState copy;
    copy.currentPlayer = this->currentPlayer;
    
    copy.players.resize(2);
    
    copy.players = this->players;
    
    // Deep copy board
    copy.board = make_shared<Board>(*(this->getBoard()));
    
    copy.bitcounter = this->bitcounter;
    
    return copy;
}

void GameState::initiateGame(){
    cout << "\n=== OTHELLO GAME SETUP ===" << endl;
    cout << "Choose game mode:" << endl;
    cout << "1. Human vs Human" << endl;
    cout << "2. Human vs Bot" << endl;
    cout << "3. Bot vs Bot" << endl;
    cout << "Enter your choice (1-3): ";
    
    int choice;
    cin >> choice;
    
    while (choice < 1 || choice > 3) {
        cout << "Invalid choice! Please enter 1, 2, or 3: ";
        cin >> choice;
    }
    
    players.clear();
    
    srand(time(nullptr));
    bool randomOrder = rand() % 2; // 0 or 1 for random order
    
    switch (choice) {
        case 1: // Human vs Human
            cout << "\nSetting up Human vs Human game..." << endl;
            addHumanPlayer();
            addHumanPlayer();
            break;
            
        case 2: // Human vs Bot
            cout << "\nSetting up Human vs Bot game..." << endl;
            if (randomOrder) {
                addHumanPlayer();      // Color 0
                addComputerPlayer();   // Color 1
                cout << "Player order: Human (Black) vs Bot (White)" << endl;
            } else {
                addComputerPlayer();   // Color 0
                addHumanPlayer();      // Color 1
                cout << "Player order: Bot (Black) vs Human (White)" << endl;
            }
            break;
            
        case 3: // Bot vs Bot
            cout << "\nSetting up Bot vs Bot game..." << endl;
            addComputerPlayer();
            addComputerPlayer(); 
            break;
    }
    board->resetBoard();
    cout << "\nGame setup complete!" << endl;
    cout << "Black pieces (●) move first, White pieces (○) move second." << endl;
    cout << "Press Enter to start the game...";
    cin.ignore(); // Clear input buffer
    cin.get();    // Wait for Enter key
    
}

void GameState::runGame(){
    initiateGame();
    piecePosition newPiecePosition;
    
    while (!isGameOver()){
        for (auto & player : players){
            currentPlayer = player->getColor();
            
            if (dynamic_cast<HumanPlayer*>(player.get()) != nullptr){
                board->printBoard();
                HumanPlayer* humanPlayer= dynamic_cast<HumanPlayer*>(player.get());
                newPiecePosition = humanPlayer->pickAction();
                
                while (board->isInValidPosition(newPiecePosition)){
                    cout << "You have entered an invalid position." << endl;
                    newPiecePosition = humanPlayer->insertPiece();
                }
                board->updateBoard(newPiecePosition);
            } 
            else if (dynamic_cast<BotPlayer*>(player.get()) != nullptr){
                BotPlayer* botPlayer= dynamic_cast<BotPlayer*>(player.get());
                newPiecePosition = botPlayer->pickAction(this);
                
                board->printBoard();
                cout << "AI Player " << currentPlayer << " plays: (" 
                     << newPiecePosition.xCoord << ", " << newPiecePosition.yCoord << ")" << endl;

                board->updateBoard(newPiecePosition);
            }

            if (isGameOver()) {
                break;
            }
        }
    }
    printWinner();
}

int GameState::getNumbBlackPieces() const{
    return bitcounter.countSetBits(board->getBlackPieces());
}

int GameState::getNumbWhitePieces() const{
    return bitcounter.countSetBits(board->getWhitePieces());
}

bool GameState::printWinner() const{
    int numbBlackPieces= bitcounter.countSetBits(board->getBlackPieces());
    int numbWhitePieces= bitcounter.countSetBits(board->getBlackPieces());

    if (numbWhitePieces> numbBlackPieces){
        cout<< "White wins";
    }
    else if(numbBlackPieces > numbWhitePieces){
        cout<< "Black wins";
    }
    else{
        cout<< "It is a tie!";
    }
}