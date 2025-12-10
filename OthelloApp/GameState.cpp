#include "GameState.h"
#include <iostream>
using namespace std;
#include <iostream>
#include <vector>
#include"BotPlayer.h"
#include"HumanPlayer.h"
#include "Constants.h"

GameState::GameState() : currentPlayer(0) {
    board = make_shared<Board>();
    bitcounter.initialize();
}

GameState::GameState(shared_ptr<Board> newBoard, int player) 
    : currentPlayer(player) {
    
    board = make_shared<Board>(*newBoard);
    bitcounter.initialize();
}

shared_ptr<Board> GameState::getBoard(){
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

void GameState::addComputerPlayer(enumDifficulty cpuDifficulty){
    int newColor=nBlack;
    if (!players.empty()) {
        newColor=nWhite;
    } 
    players.push_back(make_shared<BotPlayer>(newColor, cpuDifficulty));
}

void GameState::addHumanPlayer(){
    int newColor=nBlack;
    if (!players.empty()) {
        newColor=nWhite;
    } 
    players.push_back(make_shared<HumanPlayer>(newColor));
}

GameState GameState::deepCopy(){
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
            {
                enumDifficulty botDifficulty = selectBotDifficulty();
                if (randomOrder) {
                    addHumanPlayer();
                    addComputerPlayer(botDifficulty);
                    cout << "Player order: Human (Black) vs Bot (White) - ";
                    cout << endl;
                } else {
                    addComputerPlayer(botDifficulty);
                    addHumanPlayer();
                    cout << "Player order: Bot (Black) vs Human (White) - ";
                    cout << endl;
                }
            }
            break;
            
        case 3: // Bot vs Bot
            cout << "\nSetting up Bot vs Bot game..." << endl;
            {
                cout << "Select difficulty for Bot 1 (Black):" << endl;
                enumDifficulty bot1Difficulty = selectBotDifficulty();
                
                cout << "Select difficulty for Bot 2 (White):" << endl;
                enumDifficulty bot2Difficulty = selectBotDifficulty();
                
                addComputerPlayer(bot1Difficulty);
                addComputerPlayer(bot2Difficulty);
                
                cout << "Player order: Bot 1 (Black) - ";
                cout << " vs Bot 2 (White) - ";
                cout << endl;
            }
            break;
    }
    
    board->resetBoard();
    cout << "\nGame setup complete!" << endl;
    cout << "Black pieces (●) move first, White pieces (○) move second." << endl;
    cout << "Press Enter to start the game...";
    cin.ignore();
    cin.get();
}

enumDifficulty GameState::selectBotDifficulty() {
    cout << "Choose bot difficulty:" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    cout << "Enter your choice (1-3): ";
    
    int difficultyChoice;
    cin >> difficultyChoice;
    
    while (difficultyChoice < 1 || difficultyChoice > 3) {
        cout << "Invalid choice! Please enter 1, 2, or 3: ";
        cin >> difficultyChoice;
    }
    
    switch (difficultyChoice) {
        case 1: return easy;
        case 2: return medium;
        case 3: return hard;
        default: return easy; // fallback
    }
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
                board->printBoard();
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

vector<std::shared_ptr<Player>>& GameState::getPlayers() {
    return players;
}

void GameState::printWinner() const{
    cout<<"printing winner"<<endl;
    int numbBlackPieces= bitcounter.countSetBits(board->getBlackPieces());
    int numbWhitePieces= bitcounter.countSetBits(board->getWhitePieces());

    if (numbWhitePieces> numbBlackPieces){
         cout << "Condition 1: " << numbWhitePieces << " > " << numbBlackPieces << " = true" << endl;
        cout<< "White wins"<<endl;
    }
    else if(numbBlackPieces > numbWhitePieces){
        cout << "Condition 2: " << numbBlackPieces << " > " << numbWhitePieces << " = true" << endl;
        cout<< "Black wins"<<endl;
    }
    else{
        cout<< "It is a tie!"<<endl;
    }
    cout << "Game over. Exiting..." << endl;
}

int GameState::getWinner() const{
    int numbBlackPieces= bitcounter.countSetBits(board->getBlackPieces());
    int numbWhitePieces= bitcounter.countSetBits(board->getWhitePieces());

    if (numbWhitePieces> numbBlackPieces){
        return enumPiece::nWhite;
    }
    else if(numbBlackPieces > numbWhitePieces){
        return enumPiece::nBlack;
    }
    else{
        return DRAW_VALUE;
    }
}
