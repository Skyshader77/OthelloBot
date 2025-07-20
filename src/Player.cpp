#include "Player.h"
#include <iostream>
using namespace std;
#include <vector>

piecePosition Player::insertPiece(){
    piecePosition newPiece;
    
    cout << "Enter piece coordinates:" << endl;
    cout << "X coordinate: ";
    cin >> newPiece.xCoord;
    
    cout << "Y coordinate: ";
    cin >> newPiece.yCoord;
    
    newPiece.ncolor = myColor;
    
    cout << "Player wants piece to be placed at (" << newPiece.xCoord << ", " << newPiece.yCoord 
         << ") with color " << newPiece.ncolor << ". System will now check if placement is valid" << endl;
    
    return newPiece;
}

void Player::leaveGame(){
    char confirm;
    cout << "Are you sure you want to leave the game? (y/n): ";
    cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        cout << "Player " << myColor << " has left the game. Goodbye!" << endl;
        exit(0);
    } else {
        cout << "Continuing game..." << endl;
    }
}

piecePosition Player::pickAction(){
    int choice;
    
    cout << "\n=== Player " << myColor << "'s Turn ===" << endl;
    cout << "Choose an action:" << endl;
    cout << "1. Insert Piece" << endl;
    cout << "2. Leave Game" << endl;
    cout << "Enter your choice (1-2): ";
    cin >> choice;
    piecePosition newposition;
    switch(choice) {
        case 1:
            newposition=insertPiece();
            break;
        case 2:
            leaveGame();
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            pickAction();
            break;
    }
    return newposition;
}