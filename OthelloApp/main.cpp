#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    OthelloWindow window;
    window.show();

    return app.exec();
}
