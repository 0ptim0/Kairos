#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <unordered_map>
#include <vector>

#include "database.h"
#include "record.h"
#include "chartview.h"

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
    ChartWidget *chart;
    unsigned id = 0;

private:
    QStringList headers = {"Date", "Tags", "Hours", "Comment", ""};
    std::vector<int> row2Id;
    std::unordered_map<std::string, float> tag2Hours;
    bool m_isProgrammaticChange = false;

private:
    void updateTable();

public slots:
    void addRecord();
    void removeRecord(unsigned id);
    void changeTableValue(QTableWidgetItem *index);
};
#endif  // MAINWINDOW_H
