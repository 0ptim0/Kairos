#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyleSheet(
        "* {"
        "    color: #FFFFFF;"
        "    background-color: #333333;"
        "}"
        "*:disabled {"
        "    color: #FFFFFF;"
        "    background-color: #333333;"
        "}"
        "*:focus {"
        "    outline: none;"
        "}"
        "*:hover {"
        "    background-color: #333333;"
        "}");
    MainWindow w;
    w.show();
    return a.exec();
}
