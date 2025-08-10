#include "GameState.h"
#include "Constants.h"
#include "mainwindow.h"
#include "HumanPlayer.h"
#include "BotPlayer.h"
#include <QMenuBar>

OthelloWindow::OthelloWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Othello Game");
    setFixedSize(600, 600);

    setupUI();
    setupGame();

    botTimer = new QTimer(this);
    botTimer->setSingleShot(true);
    connect(botTimer, &QTimer::timeout, this, &OthelloWindow::processBotMove);
}

void OthelloWindow::newGame() {
    GameSetupDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        setupGameWithMode(dialog.getGameMode(),
                          dialog.getBot1Difficulty(),
                          dialog.getBot2Difficulty());
        updateDisplay();
        processCurrentPlayer();
    }
}

void OthelloWindow::onCellClicked(int row, int col) {
    if (!gameState.isGameOver()) {
        piecePosition pos = {col, row};

        auto& players = gameState.getPlayers();
        if (currentPlayerIndex < players.size()) {
            auto currentPlayer = players[currentPlayerIndex];
            if (dynamic_cast<HumanPlayer*>(currentPlayer.get()) != nullptr) {
                if (gameState.getBoard()->isInValidPosition(pos)) {
                    gameState.getBoard()->updateBoard(pos);
                    nextPlayer();
                    updateDisplay();

                    if (!gameState.isGameOver()) {
                        processCurrentPlayer();
                    } else {
                        showGameOver();
                    }
                }
            }
        }
    }
}

void OthelloWindow::setupUI() {
    auto* centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    auto* mainLayout = new QVBoxLayout(centralWidget);

    auto* menuBar = this->menuBar();
    auto* gameMenu = menuBar->addMenu("Game");
    gameMenu->addAction("New Game", this, &OthelloWindow::newGame);
    gameMenu->addAction("Exit", this, &QWidget::close);

    statusLabel = new QLabel("Welcome to Othello! Start a new game.");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-weight: bold; padding: 10px;");
    mainLayout->addWidget(statusLabel);

    auto* scoreLayout = new QHBoxLayout();
    blackScoreLabel = new QLabel("Black: 2");
    whiteScoreLabel = new QLabel("White: 2");
    blackScoreLabel->setStyleSheet("font-weight: bold;");
    whiteScoreLabel->setStyleSheet("font-weight: bold;");

    scoreLayout->addWidget(blackScoreLabel);
    scoreLayout->addStretch();
    scoreLayout->addWidget(whiteScoreLabel);
    mainLayout->addLayout(scoreLayout);

    boardWidget = new BoardWidget();
    connect(boardWidget, &BoardWidget::cellClicked,
            this, &OthelloWindow::onCellClicked);

    mainLayout->addWidget(boardWidget, 0, Qt::AlignCenter);

    auto* buttonLayout = new QHBoxLayout();
    auto* newGameButton = new QPushButton("New Game");
    connect(newGameButton, &QPushButton::clicked, this, &OthelloWindow::newGame);

    buttonLayout->addStretch();
    buttonLayout->addWidget(newGameButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
}

void OthelloWindow::processBotMove() {
    if (!gameState.isGameOver()) {
        auto& players = gameState.getPlayers();
        if (currentPlayerIndex < players.size()) {
            auto currentPlayer = players[currentPlayerIndex];
            if (auto* botPlayer = dynamic_cast<BotPlayer*>(currentPlayer.get())) {
                piecePosition pos = botPlayer->pickAction(&gameState);
                gameState.getBoard()->updateBoard(pos);

                statusLabel->setText(QString("Bot played at (%1, %2)")
                                         .arg(pos.xCoord).arg(pos.yCoord));

                nextPlayer();
                updateDisplay();

                if (!gameState.isGameOver()) {
                    botTimer->start(1000);
                } else {
                    showGameOver();
                }
            }
        }
    }
}

void OthelloWindow::processCurrentPlayer() {
    if (!gameState.isGameOver()) {
        auto& players = gameState.getPlayers();
        if (currentPlayerIndex < players.size()) {
            auto currentPlayer = players[currentPlayerIndex];

            if (dynamic_cast<HumanPlayer*>(currentPlayer.get()) != nullptr) {
                statusLabel->setText(QString("Player %1's turn (Human)")
                                         .arg(currentPlayer->getColor() == nBlack ? "Black" : "White"));
            } else if (dynamic_cast<BotPlayer*>(currentPlayer.get()) != nullptr) {
                statusLabel->setText(QString("Player %1's turn (Bot thinking...)")
                                         .arg(currentPlayer->getColor() == nBlack ? "Black" : "White"));
                botTimer->start(500);
                processBotMove();
            }
        }
    }
}

void OthelloWindow::setupGame() {
    gameState = GameState();
    currentPlayerIndex = 0;
}

void OthelloWindow::setupGameWithMode(int mode, enumDifficulty bot1Diff, enumDifficulty bot2Diff) {
    gameState = GameState();
    currentPlayerIndex = 0;

    switch (mode) {
    case 1:
        gameState.addHumanPlayer();
        gameState.addHumanPlayer();
        break;

    case 2:
        if (rand() % 2) {
            gameState.addHumanPlayer();
            gameState.addComputerPlayer(bot1Diff);
        } else {
            gameState.addComputerPlayer(bot1Diff);
            gameState.addHumanPlayer();
        }
        break;

    case 3:
        gameState.addComputerPlayer(bot1Diff);
        gameState.addComputerPlayer(bot2Diff);
        break;
    }
    gameState.getBoard()->resetBoard();
}

void OthelloWindow::nextPlayer() {
    currentPlayerIndex = 1 - currentPlayerIndex;
    gameState.changeCurrentPlayer();
}

void OthelloWindow::updateDisplay() {
    boardWidget->setGameState(&gameState);

    int blackCount = gameState.getNumbBlackPieces();
    int whiteCount = gameState.getNumbWhitePieces();

    blackScoreLabel->setText(QString("Black: %1").arg(blackCount));
    whiteScoreLabel->setText(QString("White: %1").arg(whiteCount));
}

void OthelloWindow::showGameOver() {
    int blackCount = gameState.getNumbBlackPieces();
    int whiteCount = gameState.getNumbWhitePieces();

    QString winner;
    if (blackCount > whiteCount) {
        winner = "Black wins!";
    } else if (whiteCount > blackCount) {
        winner = "White wins!";
    } else {
        winner = "It's a tie!";
    }

    QString message = QString("%1\n\nFinal Score:\nBlack: %2\nWhite: %3")
                          .arg(winner).arg(blackCount).arg(whiteCount);

    QMessageBox::information(this, "Game Over", message);
    statusLabel->setText("Game Over - " + winner);
}
