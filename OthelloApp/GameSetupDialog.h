#ifndef GAMESETUPDIALOG_H
#define GAMESETUPDIALOG_H

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
#include "Constants.h"

class GameSetupDialog : public QDialog {
    Q_OBJECT

public:
    GameSetupDialog(QWidget* parent = nullptr);
    
    int getGameMode() const { return gameMode->checkedId(); }
    enumDifficulty getBot1Difficulty() const { 
        return static_cast<enumDifficulty>(bot1Difficulty->currentData().toInt()); 
    }
    enumDifficulty getBot2Difficulty() const { 
        return static_cast<enumDifficulty>(bot2Difficulty->currentData().toInt()); 
    }

private slots:
    void onGameModeChanged(int mode) {
        if (mode == 3) { // Bot vs Bot
            bot2Label->show();
            bot2Difficulty->show();
        } else {
            bot2Label->hide();
            bot2Difficulty->hide();
        }
    }

private:
    QButtonGroup* gameMode;
    QComboBox* bot1Difficulty;
    QComboBox* bot2Difficulty;
    QLabel* bot2Label;
};
#endif
