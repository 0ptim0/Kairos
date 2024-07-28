#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QDate>

struct Record {
    unsigned id;
    QString date;
    QString tags;
    float hours;
    QString comment;
};

#endif // RECORD_H
