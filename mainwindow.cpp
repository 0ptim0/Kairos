#include "mainwindow.h"

#include <QDir>
#include <QTableView>
#include <QVBoxLayout>
#include <algorithm>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QDate date = QDate::currentDate();
    ui->dateEdit->setDate(date);

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addRecord);
    connect(ui->table, &QTableWidget::itemChanged, this,
            &MainWindow::changeTableValue);

    QString homePath = QDir::homePath();
    QString kairosPath = homePath + "/.kairos";
    QDir dir(kairosPath);

    if (!dir.exists()) {
        if (dir.mkpath(".")) {
            qDebug() << "Creating ~/.kairos";
        } else {
            qDebug() << "Failed to create ~/.kairos directory";
        }
    }
    kairosPath = kairosPath + "/database.db";
    db.connect(kairosPath);

    chart = new ChartWidget();
    if (ui->chart->layout() == nullptr) {
        QVBoxLayout *layout = new QVBoxLayout(ui->chart);
        layout->setAlignment(chart, Qt::AlignLeft);
        layout->addWidget(chart);
    } else {
        ui->chart->layout()->addWidget(chart);
    }

    updateTable();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addRecord() {
    if (ui->tagsEdit->text().trimmed().isEmpty()) {
        ui->tagsEdit->setStyleSheet("QTextEdit { border: 2px solid red; }");
    } else if (ui->hoursEdit->text().trimmed().isEmpty()) {
        ui->hoursEdit->setStyleSheet("QTextEdit { border: 2px solid red; }");
    } else if (ui->commentEdit->text().trimmed().isEmpty()) {
        ui->commentEdit->setStyleSheet("QTextEdit { border: 2px solid red; }");
    } else {
        ui->tagsEdit->setStyleSheet("");
        ui->hoursEdit->setStyleSheet("");
        ui->commentEdit->setStyleSheet("");
        Record record = {
            .id = id,
            .date = ui->dateEdit->date().toString(Qt::ISODate),
            .tags = ui->tagsEdit->text(),
            .hours = ui->hoursEdit->text().toFloat(),
            .comment = ui->commentEdit->text(),
        };
        if (db.insertRecord(record)) {
            ++id;
            updateTable();
        }
    }
}

void MainWindow::updateTable() {
    auto table = db.getRecords();

    m_isProgrammaticChange = true;

    ui->table->setRowCount(table.size());
    ui->table->setColumnCount(5);
    ui->table->setHorizontalHeaderLabels(headers);


    std::sort(table.begin(), table.end(), [](const Record &a, const Record &b) {
        return QDate::fromString(a.date, Qt::ISODate) >
               QDate::fromString(b.date, Qt::ISODate);
    });

    // That's bad
    for (int row = 0; row < ui->table->rowCount(); ++row) {
        QWidget *removeButton = ui->table->cellWidget(row, 4);
        if (removeButton) {
            removeButton->deleteLater();
            ui->table->setCellWidget(row, 4, nullptr);
        }
    }

    row2Id.clear();
    tag2Hours.clear();

    unsigned maxId = 0;

    for (int row = 0; row < table.size(); ++row) {
        auto id = table[row].id;
        row2Id.push_back(id);

        ui->table->setItem(row, 0, new QTableWidgetItem(table[row].date));
        ui->table->setItem(row, 1, new QTableWidgetItem(table[row].tags));
        ui->table->setItem(
            row, 2,
            new QTableWidgetItem(QString::number(table[row].hours, 'f', 2)));
        ui->table->setItem(row, 3, new QTableWidgetItem(table[row].comment));
        QPushButton *removeButton = new QPushButton();
        removeButton->setIcon(QIcon::fromTheme("edit-delete"));
        removeButton->setIconSize(QSize(16, 16));
        connect(removeButton, &QPushButton::clicked, this,
                [this, id]() { removeRecord(id); });
        ui->table->setCellWidget(row, 4, removeButton);
        ui->table->setColumnWidth(4, 16);
        ui->table->setColumnWidth(3, 620);
    
        tag2Hours[table[row].tags] += table[row].hours;

        maxId = std::max(maxId, id);
    }
    
    if (!isIdInit) {
        id = maxId + 1;
        isIdInit = true;
    }

    chart->update(tag2Hours);

    m_isProgrammaticChange = false;
}

void MainWindow::removeRecord(unsigned id) {
    m_isProgrammaticChange = true;
    if (db.removeRecord(id)) {
        updateTable();
    }
    m_isProgrammaticChange = false;
}

void MainWindow::changeTableValue(QTableWidgetItem *index) {
    if (m_isProgrammaticChange) {
        return;
    }
    auto row = index->row();
    if (row > row2Id.size()) {
        qDebug() << "Wrong row";
        return;
    }
    auto id = row2Id[row];
    auto column = index->column();
    auto value = index->text();
    db.updateRecordById(id, headers[column], value);
    updateTable();
}
