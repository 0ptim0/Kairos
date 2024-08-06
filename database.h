#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <unordered_map>

#include "record.h"

class DataBase {
public:
    bool connect(const QString& path) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(path);

        if (!db.open()) {
            qDebug() << "Error: Unable to open the database!";
            qDebug() << db.lastError().text();
            return false;
        }

        qDebug() << "Database connection established successfully";

        QSqlQuery query;
        QString createTableQuery = R"(
            CREATE TABLE IF NOT EXISTS records (
                id UNSIGNED,
                date TEXT NOT NULL,
                tags TEXT,
                hours UNSIGNED,
                comment TEXT
            )
        )";

        if (!query.exec(createTableQuery)) {
            qDebug() << "Error: Unable to create table!";
            qDebug() << query.lastError().text();
            return false;
        }
        return true;
    }

    bool insertRecord(const Record& record) {
        QSqlQuery query;
        query.prepare(
            "INSERT INTO records (id, date, tags, hours, comment) VALUES (:id, "
            ":date, :tags, :hours, :comment)");
        query.bindValue(":id", record.id);
        query.bindValue(":date", record.date);
        query.bindValue(":tags", record.tags);
        query.bindValue(":hours", record.hours);
        query.bindValue(":comment", record.comment);
        if (!query.exec()) {
            qDebug() << "Error: Unable to insert record!";
            qDebug() << query.lastError().text();
            return false;
        }
        return true;
    }

    std::vector<Record> getRecords() {
        std::vector<Record> records;
        QSqlQuery query;
        if (!query.exec("SELECT id, date, tags, hours, comment FROM records")) {
            qDebug() << "Error: Unable to execute query!";
            qDebug() << query.lastError().text();
        } else {
            while (query.next()) {
                unsigned id = query.value(0).toUInt();
                QString date = query.value(1).toString();
                QString tags = query.value(2).toString();
                float hours = query.value(3).toFloat();
                QString comment = query.value(4).toString();
                qDebug() << id << date << tags << hours << comment;
                Record record = {.id = id,
                                 .date = date,
                                 .tags = tags,
                                 .hours = hours,
                                 .comment = comment};
                records.push_back(record);
            }
        }
        return records;
    }

    bool removeRecord(unsigned id) {
        QSqlQuery query;
        query.prepare("DELETE FROM records WHERE id = :id");
        query.bindValue(":id", id);

        if (!query.exec()) {
            qDebug() << "Error: Unable to delete record!";
            qDebug() << query.lastError().text();
            return false;
        }
        return true;
    }

    bool updateRecordById(int id, const QString& col, const QString& newValue) {
        QSqlQuery query;

        QString query_str = "UPDATE records SET ";
        query_str += col;
        query_str += " = :newValue WHERE id = :id";

        query.prepare(query_str);
        query.bindValue(":newValue", newValue);
        query.bindValue(":id", id);

        if (!query.exec()) {
            qDebug() << "Update failed:" << query.lastError();
            return false;
        }
        return true;
    }
};
