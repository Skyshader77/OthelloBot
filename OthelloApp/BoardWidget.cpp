#include <QPixmap>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include "Constants.h"
#include "BoardWidget.h"

BoardWidget::BoardWidget(QWidget* parent) : QFrame(parent), gameState(nullptr) {
    setFixedSize(480, 480);
    setFrameStyle(QFrame::Box);
    setLineWidth(2);
    setMouseTracking(true);
    
    loadImages();
}

void BoardWidget::setGameState(GameState* gs) {
    gameState = gs;
    update();
}

void BoardWidget::loadImages() {
    QString appDir = QCoreApplication::applicationDirPath();
    
    QString imagesPath = QDir(appDir).absoluteFilePath("../images");

    QStringList possiblePaths = {
        QDir(appDir).absoluteFilePath("../images"),
        QDir(appDir).absoluteFilePath("images"),
        QDir(appDir).absoluteFilePath("../../images"),
        ":/images"
    };
    
    bool blackLoaded = false, whiteLoaded = false;
    
    for (const QString& basePath : possiblePaths) {
        if (!blackLoaded) {
            QString blackPath = QDir(basePath).absoluteFilePath(QString::fromStdString(BLACK_IMAGE));
            if (blackPieceImage.load(blackPath)) {
                blackLoaded = true;
                qDebug() << "Loaded black piece from:" << blackPath;
            }
        }
        
        if (!whiteLoaded) {
            QString whitePath = QDir(basePath).absoluteFilePath(QString::fromStdString(WHITE_IMAGE));
            if (whitePieceImage.load(whitePath)) {
                whiteLoaded = true;
                qDebug() << "Loaded white piece from:" << whitePath;
            }
        }
        
        if (blackLoaded && whiteLoaded) break;
    }
    
    if (!blackLoaded) {
        qDebug() << "Failed to load Black.png - will use fallback";
    }
    if (!whiteLoaded) {
        qDebug() << "Failed to load White.png - will use fallback";
    }
}

void BoardWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    painter.setPen(QPen(Qt::black, 1));
    
    const int cellSize = 60;
    for (int i = 0; i <= BOARD_LENGTH; ++i) {
        painter.drawLine(0, i * cellSize, BOARD_LENGTH * cellSize, i * cellSize);
        painter.drawLine(i * cellSize, 0, i * cellSize, BOARD_LENGTH * cellSize);
    }
    
    if (!gameState) return;
    
    auto board = gameState->getBoard();
    
    for (int row = 0; row < BOARD_LENGTH; ++row) {
        for (int col = 0; col < BOARD_LENGTH; ++col) {
            piecePosition pos;
            pos.xCoord=row;
            pos.yCoord=col;
            
            QRect cellRect(col * cellSize + 5, row * cellSize + 5, 
                          cellSize - 10, cellSize - 10);
            
            if (board->getPieceColorAt(pos.xCoord, pos.yCoord) == nBlack) {
                if (!blackPieceImage.isNull()) {
                    QPixmap scaledBlack = blackPieceImage.scaled(
                        cellRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    
                    QPoint drawPos = cellRect.center() - QPoint(scaledBlack.width()/2, scaledBlack.height()/2);
                    painter.drawPixmap(drawPos, scaledBlack);
                } else {
                    painter.setBrush(Qt::black);
                    painter.setPen(Qt::black);
                    painter.drawEllipse(cellRect);
                }
            } else if (board->getPieceColorAt(pos.xCoord, pos.yCoord) == nWhite) {
                if (!whitePieceImage.isNull()) {
                    QPixmap scaledWhite = whitePieceImage.scaled(
                        cellRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    
                    QPoint drawPos = cellRect.center() - QPoint(scaledWhite.width()/2, scaledWhite.height()/2);
                    painter.drawPixmap(drawPos, scaledWhite);
                } else {
                    painter.setBrush(Qt::white);
                    painter.setPen(Qt::black);
                    painter.drawEllipse(cellRect);
                }
            }
            
            if (!board->isInValidPosition(pos)) {
                painter.setBrush(Qt::transparent);
                painter.setPen(QPen(Qt::green, 2, Qt::DashLine));
                painter.drawEllipse(cellRect);
            }
        }
    }
}

void BoardWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        const int cellSize = 60;
        int col = event->x() / cellSize;
        int row = event->y() / cellSize;
            
        if (col >= 0 && col < 8 && row >= 0 && row < 8) {
                emit cellClicked(row, col);
        }
    }
}
