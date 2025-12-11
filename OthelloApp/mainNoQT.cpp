#include "GameState.h"
#include <memory>
#include <iostream>
#include <sstream>
#include"Logger.h"

using namespace std;

int main() {
    Logger logger("logfile.txt");
    GameState gamestate;
    gamestate.runGame();
}