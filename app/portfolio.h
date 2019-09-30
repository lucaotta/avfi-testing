#pragma once

#include <QDate>
#include <QString>
#include <QVector>

struct Transaction {
    QString company;
    int quantity;
    QDate date;
};

class IRepository {
public:
    virtual QVector<Transaction> all() = 0;
    virtual double currentValue(QString company) = 0;
};

class Portfolio {
public:
    const QString reportHeader = "company | shares | current price | current value | last operation";

    Portfolio(IRepository* repo);
    QString currentState();

private:
    IRepository* m_repo;
};
