#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QComboBox>
#include <QDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QFrame>
#include <memory>
#include "GameState.h"
#include "GameSetupDialog.h"
#include "BoardWidget.h"
#include "Constants.h"

class OthelloWindow : public QMainWindow {
    Q_OBJECT

public:
    OthelloWindow(QWidget* parent = nullptr) : QMainWindow(parent) {}

private slots:
    void newGame() {}
    
    void onCellClicked(int row, int col) {}
    
    void processBotMove() {}
    
    void processCurrentPlayer() {}

private:
    void setupUI() {}
    
    void setupGame() {}
    
    void setupGameWithMode(int mode, enumDifficulty bot1Diff, enumDifficulty bot2Diff) {}
    
    void nextPlayer() {}
    
    void updateDisplay() {}
    
    void showGameOver() {}

private:
    GameState gameState;
    BoardWidget* boardWidget;
    QLabel* statusLabel;
    QLabel* blackScoreLabel;
    QLabel* whiteScoreLabel;
    QTimer* botTimer;
};
