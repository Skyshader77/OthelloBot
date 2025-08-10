#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

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
#include <QPixmap>
#include <memory>
#include "GameState.h"

class BoardWidget : public QFrame {
    Q_OBJECT

public:
    BoardWidget(QWidget* parent = nullptr);
    void setGameState(GameState* gs);

signals:
    void cellClicked(int row, int col);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    GameState* gameState;
    QPixmap blackPieceImage;
    QPixmap whitePieceImage;
    
    void loadImages();
};
#endif
