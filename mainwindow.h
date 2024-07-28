#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <unordered_map>
#include <vector>

#include "database.h"
#include "record.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DataBase db;
    unsigned id = 0;

private:
    void updateTable();

public slots:
    void addRecord();
    void removeRecord(unsigned id);
};
#endif  // MAINWINDOW_H
